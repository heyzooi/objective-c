#import <Foundation/Foundation.h>
#import "PNStructures.h"


#pragma mark Class forward

@class PubNub;


/**
 @brief      Class which allow to manage subscribe loop.
 @discussion Track subscription and time token information. Subscriber manage recovery as well.
 
 @author Sergey Mamontov
 @since 4.0
 @copyright © 2009-2015 PubNub, Inc.
 */
@interface PNSubscriber : NSObject


///------------------------------------------------
/// @name State Information and Manipulation
///------------------------------------------------

/**
 @brief  Retrieve list of all remote data objects names to which client subscriber at this moment.
 
 @return Object names list.
 
 @since 4.0
 */
- (NSArray *)allObjects;

/**
 @brief  List of channels.
 
 @return Return list of channels on which client subscribed at this moment.
 
 @since 4.0
 */
- (NSArray *)channels;

/**
 @brief  List of channel groups.
 
 @return Return list of channel groups on which client subscribed at this moment.
 
 @since 4.0
 */
- (NSArray *)channelGroups;

/**
 @brief  List of presence channels.
 
 @return Return list of presence channels for which client observing for presence events.
 
 @since 4.0
 */
- (NSArray *)presenceChannels;


///------------------------------------------------
/// @name Initialization and Configuration
///------------------------------------------------

/**
 @brief  Construct subscribe loop manager for concrete \b PubNub client.
 
 @param client Reference on client which will be weakly stored in subscriber.
 
 @return Configured and ready to use subscribe manager instance.
 
 @since 4.0
 */
+ (instancetype)subscriberForClient:(PubNub *)client;


///------------------------------------------------
/// @name Subscription information modification
///------------------------------------------------

/**
 @brief  Add new channels to the list at which client subscribed.
 
 @param channels List of channels which should be added to the list.
 
 @since 4.0
 */
- (void)addChannels:(NSArray *)channels;

/**
 @brief  Remove channels from the list on which client subscribed.
 
 @param channels List of channels which should be removed from the list.
 
 @since 4.0
 */
- (void)removeChannels:(NSArray *)channels;

/**
 @brief  Add new channel groups to the list at which client subscribed.
 
 @param groups List of channel groups which should be added to the list.
 
 @since 4.0
 */
- (void)addChannelGroups:(NSArray *)groups;

/**
 @brief  Remove channel groups from the list on which client subscribed.
 
 @param groups List of channel groups which should be removed from the list.
 
 @since 4.0
 */
- (void)removeChannelGroups:(NSArray *)groups;

/**
 @brief  Add new presence channels to the list at which client subscribed.
 
 @param presenceChannels List of presence channels which should be added to the list.
 
 @since 4.0
 */
- (void)addPresenceChannels:(NSArray *)presenceChannels;

/**
 @brief  Remove presence channels from the list on which client subscribed.
 
 @param presenceChannels List of presence channels which should be removed from the list.
 
 @since 4.0
 */
- (void)removePresenceChannels:(NSArray *)presenceChannels;


///------------------------------------------------
/// @name Subscription
///------------------------------------------------

/**
 @brief      Perform initial subscription with \b 0 timetoken.
 @discussion Subscription with \b 0 timetoken "register" client in \b PubNub network and allow to
             receive live updates from remote data objects live feed.
 
 @param initialSubscribe Stores whether client trying to subscriber using \b 0 time token and
                         trigger all required presence notifications or  not.
 @param state            Reference on client state which should be bound to channels on which
                         client has been subscribed or will subscribe now.
 @param block            Subscription process completion block which pass only one
                         argument - request processing status to report about how subscription was
                         successful or not.
 
 @since 4.0
 */
- (void)subscribe:(BOOL)initialSubscribe withState:(NSDictionary *)state
       completion:(PNStatusBlock)block;

/**
 @brief  Try restore subscription cycle by using \b 0 time token and if required try to catch up on
         previous subscribe time token (basing on user configuration).
 
 @since 4.0
 */
- (void)restoreSubscriptionCycleIfRequired;

/**
 @brief      Perform unsubscription operation.
 @discussion If suitable objects has been passed, then client will ask \b PubNub presence service to
             trigger \c 'leave' presence events on passed objects.
 
 @param channels Whether unsubscribing from list of channels or channel groups.
 @param objects  List of objects from which client should unsubscribe.
 @param block    Unsubscription process completion block which pass only one argument - request
                 processing status to report about how unsubscription was successful or not.
 
 @since 4.0
 */
- (void)unsubscribeFrom:(BOOL)channels objects:(NSArray *)objects
         withCompletion:(PNStatusBlock)block;

#pragma mark -


@end
