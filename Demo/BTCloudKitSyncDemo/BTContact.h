//
//  BTContact.h
//  BTCloudKitSyncDemo
//
//  Created by Boyd Timothy on 3/19/16.
//  Copyright © 2016 Boyd Timothy.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#import <Foundation/Foundation.h>

@interface BTContact : NSObject

/**
 The identifier is a UUID. This identifier is used locally and also used when
 synchronizing objects with CloudKit.
 */
@property (nonatomic, strong) NSString *identifier;
@property (nonatomic, strong) NSString *firstName;
@property (nonatomic, strong) NSString *lastName;
@property (nonatomic, strong) NSDate *lastModified;

- (NSString *)fullName;

- (NSDictionary *)dictionaryRepresentation;
+ (BTContact *)contactWithDictionary:(NSDictionary *)contactInfo;

@end
