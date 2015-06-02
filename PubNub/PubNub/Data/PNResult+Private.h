/**
 @author Sergey Mamontov
 @since 4.0
 @copyright © 2009-2015 PubNub, Inc.
 */
#import "PNResult.h"


#pragma mark Private interface declaration

@interface PNResult () <NSCopying>


///------------------------------------------------
/// @name Information
///------------------------------------------------

@property (nonatomic, assign) NSInteger statusCode;
@property (nonatomic, assign) PNOperationType operation;
@property (nonatomic, assign, getter = isTLSEnabled) BOOL TLSEnabled;
@property (nonatomic, copy) NSString *uuid;
@property (nonatomic, copy) NSString *authKey;
@property (nonatomic, copy) NSString *origin;
@property (nonatomic, copy) NSURLRequest *clientRequest;
@property (nonatomic, copy) NSDictionary *data;


///------------------------------------------------
/// @name Initialization and configuration
///------------------------------------------------

/**
 @brief  Consntruct result instance in response to successful task completion.
 
 @param operation     One of \b PNOperationType enum fields to describe what kind of operation has
                      been processed.
 @param task          Reference on data task which has been used to communicate with \b PubNub
                      network.
 @param processedData Reference on data which has been loaded and pre-processed by corresponding
                      parser.
 
 @return Constructed and ready to use result instance.
 
 @since 4.0
 */
+ (instancetype)objectForOperation:(PNOperationType)operation
                 completedWithTaks:(NSURLSessionDataTask *)task
                     processedData:(NSDictionary *)processedData;

/**
 @brief  Initialize result instance in response to successful task completion.
 
 @param operation     One of \b PNOperationType enum fields to describe what kind of operation has
                      been processed.
 @param task          Reference on data task which has been used to communicate with \b PubNub
                      network.
 @param processedData Reference on data which has been loaded and pre-processed by corresponding
                      parser.
 
 @return Initialized and ready to use result instance.
 
 @since 4.0
 */
- (instancetype)initForOperation:(PNOperationType)operation
               completedWithTaks:(NSURLSessionDataTask *)task
                   processedData:(NSDictionary *)processedData NS_DESIGNATED_INITIALIZER;

/**
 @brief      Make copy of current result object with mutated data which should be stored in it.
 @discussion Method can be used to create sub-events (for example one for each message or presence 
             event).
 
 @param data Reference on data which should be stored within new instance.
 
 @return Copy of receiver with modified data.
 
 @since 4.0
 */
- (instancetype)copyWithMutatedData:(id)data;


///------------------------------------------------
/// @name Misc
///------------------------------------------------

/**
 @brief  Convert result object to dictionary which can be used to print out structured data
 
 @return Object in dictionary representation.
 
 @since 4.0
 */
- (NSDictionary *)dictionaryRepresentation;

/**
 @brief  Convert result object to string which can be used to print out data
 
 @return Stringified object representation.
 
 @since 4.0
 */
- (NSString *)stringifiedRepresentation;

#pragma mark -


@end
