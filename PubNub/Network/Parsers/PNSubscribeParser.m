/**
 @author Sergey Mamontov
 @since 4.0
 @copyright © 2009-2015 PubNub, Inc.
 */
#import "PNSubscribeParser.h"
#import "PubNub+CorePrivate.h"
#import "PNHelpers.h"
#import "PNAES.h"


#pragma mark Static

/**
 Stores reference on index under which events list is stored.
 */
static NSUInteger const kPNEventsListElementIndex = 0;

/**
 Stores reference on time token element index in response for events.
 */
static NSUInteger const kPNEventTimeTokenElement = 1;

/**
 Stores reference on index under which channels list is stored.
 */
static NSUInteger const kPNEventChannelsElementIndex = 2;

/**
 @brief Stores reference on index under which channels detalization is stored
 
 @discussion In case if under \c kPNEventChannelsElementIndex stored list of channel groups, under 
             this index will be stored list of actual channels from channel group at which event
             fired.
 
 @since 3.7.0
 */
static NSUInteger const kPNEventChannelsDetailsElementIndex = 3;


#pragma mark - Protected interface

@interface PNSubscribeParser ()


#pragma mark - Events processing

/**
 @brief  Parse real-time event received from data object live feed.
 
 @param data           Reference on service-provided data about event.
 @param channel        Reference on channel for which event has been received.
 @param channelGroup   Reference on channel group for which event has been received.
 @param additionalData Additional information provided by client to complete parsing.
 
 @return Pre-processed event information (depending on stored data).
 
 @since 4.0
 */
+ (NSMutableDictionary *)eventFromData:(id)data forChannel:(NSString *)channel
                                 group:(NSString *)channelGroup
              withAdditionalParserData:(NSDictionary *)additionalData;

/**
 @brief  Parse provided data as new message event.
 
 @param data           Data which should be parsed to required 'message' object format.
 @param additionalData Additional information provided by client to complete parsing.
 
 @return Processed and parsed 'message' object.
 
 @since 4.0
 */
+ (NSMutableDictionary *)messageFromData:(id)data
                withAdditionalParserData:(NSDictionary *)additionalData;

/**
 @brief  Parse provded data as presence event.
 
 @param data Data which should be parsed to required 'presence event' object format.
 
 @return Processed and parsed 'presence event' object.
 
 @since 4.0
 */
+ (NSMutableDictionary *)presenceFromData:(NSDictionary *)data;

#pragma mark -


@end


#pragma mark - Interface implementation

@implementation PNSubscribeParser


#pragma mark - Identification

+ (NSArray *)operations {
    
    return @[@(PNSubscribeOperation)];
}

+ (BOOL)requireAdditionalData {
    
    return YES;
}


#pragma mark - Parsing

+ (NSDictionary *)parsedServiceResponse:(id)response withData:(NSDictionary *)additionalData {
    
    // To handle case when response is unexpected for this type of operation processed value sent
    // through 'nil' initialized local variable.
    NSDictionary *processedResponse = nil;
    
    // Array will arrive in case of subscription event
    if ([response isKindOfClass:[NSArray class]]) {
        
        NSArray *feedEvents = response[kPNEventsListElementIndex];
        NSNumber *timeToken = @([response[kPNEventTimeTokenElement] longLongValue]);
        NSArray *channels = nil;
        NSArray *groups = nil;
        if ([(NSArray *)response count] > kPNEventChannelsElementIndex) {
            
            channels = [PNChannel namesFromRequest:response[kPNEventChannelsElementIndex]];
        }
        if ([(NSArray *)response count] > kPNEventChannelsDetailsElementIndex) {
            
            groups = [PNChannel namesFromRequest:response[kPNEventChannelsDetailsElementIndex]];
        }
        
        // Checking whether at least one event arrived or not.
        if ([feedEvents count]) {
            
            NSMutableArray *events = [[NSMutableArray alloc] initWithCapacity:[feedEvents count]];
            for (NSUInteger eventIdx = 0; eventIdx < [feedEvents count]; eventIdx++) {
                
                // Fetching remote data object name on which event fired.
                NSString *objectName = (eventIdx < [channels count] ? channels[eventIdx] : channels[0]);
                NSString *groupName = ([groups count] > eventIdx ? groups[eventIdx] : nil);
                NSMutableDictionary *event = [self eventFromData:feedEvents[eventIdx]
                                                      forChannel:objectName group:groupName
                                        withAdditionalParserData:additionalData];
                event[@"timetoken"] = timeToken;
                [events addObject:event];
            }
            feedEvents = [events copy];
        }
        processedResponse = [PNDictionary dictionaryWithDictionary:@{
                             @"events":feedEvents,@"timetoken":timeToken}];
    }
    
    return processedResponse;
}


#pragma mark - Events processing

+ (NSMutableDictionary *)eventFromData:(id)data forChannel:(NSString *)channel
                                 group:(NSString *)channelGroup
              withAdditionalParserData:(NSDictionary *)additionalData {
    
    NSMutableDictionary *event = [PNDictionary new];
    if ([channel length]) {
        
        event[(![channelGroup length] ? @"subscribedChannel": @"actualChannel")] = channel;
    }
    if ([channelGroup length]) {
        
        event[@"subscribedChannel"] = channelGroup;
    }
    
    BOOL isPresenceEvent = [PNChannel isPresenceObject:channel];
    if (![channel length] && [data isKindOfClass:[NSDictionary class]]) {
        
        isPresenceEvent = (data[@"action"] != nil && data[@"timestamp"] != nil);
    }
    
    if (isPresenceEvent) {
        
        [event addEntriesFromDictionary:[self presenceFromData:data]];
    }
    else {
        
        [event addEntriesFromDictionary:[self messageFromData:data
                                     withAdditionalParserData:additionalData]];
    }
    
    return event;
}

+ (NSMutableDictionary *)messageFromData:(id)data
                withAdditionalParserData:(NSDictionary *)additionalData {
    
    NSMutableDictionary *message = [PNDictionary dictionaryWithDictionary:@{@"message":data}];
    // Try decrypt message body if possible.
    if ([data isKindOfClass:[NSString class]] && [(NSString *)additionalData[@"cipherKey"] length]){
        
        NSError *decryptionError;
        NSData *eventData = [PNAES decrypt:data withKey:additionalData[@"cipherKey"]
                                  andError:&decryptionError];
        NSString *encryptedEventData = nil;
        if (eventData) {
            
            encryptedEventData = [[NSString alloc] initWithData:eventData
                                                       encoding:NSUTF8StringEncoding];
        }
        
        // In case if after encryption another object has been received client
        // should try to de-serialize it again as JSON object.
        if (encryptedEventData && ![encryptedEventData isEqualToString:data]) {
            
            message[@"message"] = [PNJSON JSONObjectFrom:encryptedEventData withError:nil];
        }
        
        if (decryptionError) {
            
            DDLogAESError(@"<PubNub> Message decryption error: %@", decryptionError);
            message[@"decryptError"] = @YES;
        }
    }
    
    return message;
}

+ (NSMutableDictionary *)presenceFromData:(NSDictionary *)data {
    
    NSMutableDictionary *presence = [PNDictionary new];
    
    // Processing common for all presence events data.
    presence[@"presenceEvent"] = data[@"action"];
    presence[@"presence"] = [PNDictionary new];
    presence[@"presence"][@"timetoken"] = data[@"timestamp"];
    if (data[@"uuid"]) {
        
        presence[@"presence"][@"uuid"] = data[@"uuid"];
    }
    
    // Check whether this is not state modification event.
    if (![presence[@"presenceEvent"] isEqualToString:@"state-change"]) {
        
        presence[@"presence"][@"occupancy"] = data[@"occupancy"];
    }
    else {
        
        presence[@"presence"][@"state"] = data[@"data"];
    }
    
    return presence;
}

#pragma mark -


@end
