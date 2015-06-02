#import <Foundation/Foundation.h>


/**
 @brief      Wrapper class around parameters which should be applied on resource path and query 
             string.
 @discussion Used to help builder identify what parameters related to resource path components and
             what should be used with request query composition.
 
 @author Sergey Mamontov
 @since 4.0
 @copyright © 2009-2015 PubNub, Inc.
 */
@interface PNRequestParameters : NSObject


///------------------------------------------------
/// @name Information
///------------------------------------------------

/**
 @brief  Stores reference on key/value pairs which should be expanded in remote resource path.
 
 @since 4.0
 */
@property (nonatomic, readonly) NSDictionary *pathComponents;

/**
 @brief  Stores reference on key/value pairst which should be expanded in query string.
 
 @since 4.0
 */
@property (nonatomic, readonly) NSDictionary *query;


///------------------------------------------------
/// @name Path components manipulation
///------------------------------------------------

/**
 @brief      Add resource path component for placeholder.
 @discussion Placeholder will be placed in request template with specified value.
 
 @param component            Path component value.
 @param componentPlaceholder Name of placeholder instead of which value should be placed.
 
 @since 4.0
 */
- (void)addPathComponent:(NSString *)component forPlaceholder:(NSString *)componentPlaceholder;

/**
 @brief      Add resource path components in placeholder/value format with dictionary.
 @discussion Corresponding placeholder will be placed in request template with specified value.
 
 @param components Disctionary with placeholder name / value pairs.
 
 @since 4.0
 */
- (void)addPathComponents:(NSDictionary *)components;


///------------------------------------------------
/// @name Query fields manipulation
///------------------------------------------------

/**
 @brief  Add query parameter value for specified name.
 
 @param parameter          Query parameter value.
 @param parameterFieldName Query parameter field name.
 
 @since 4.0
 */
- (void)addQueryParameter:(NSString *)parameter forFieldName:(NSString *)parameterFieldName;

/**
 @brief      Add query parameters in field name / value format with dictionary.
 
 @param parameters Disctionary with field name / value pairs.
 
 @since 4.0
 */
- (void)addQueryParameters:(NSDictionary *)parameters;

#pragma mark -


@end
