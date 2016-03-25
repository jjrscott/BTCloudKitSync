# BTCloudKitSync
A class that provides simple [CloudKit](https://developer.apple.com/icloud/) sync for iOS apps with a local cache/database.

The purpose of BTCloudKitSync is to add sync a local cache of data through a private CloudKit database ([CKDatabase](https://developer.apple.com/library/ios/documentation/CloudKit/Reference/CloudKit_Framework_Reference/index.html)).

## WORK IN PROGRESS (ITEMS TO-DO)

There are a number of things that are not yet implemented in BTCloudKitSync:

* Robust CloudKit Error Handling
* Still need to figure out how to deal with a server busy error
* When uploading CKRecord objects, it's possible that CloudKit will respond with CKErrorLimitExceeded. The code should chop the batch in half and upload the changes separately (and presumably this could happen again for each of the smaller batches)
* Figure out how to deal with CKErrorBatchRequestFailed
* Figure out how to deal with CKErrorQuotaExceeded
* Add conflict resolution during uploads
  * Check for CKErrorServerRecordChanged and figure out what the best policy is for changes. Should client always win? Use the keys here to get the records involved in the conflict: <https://developer.apple.com/library/ios/documentation/CloudKit/Reference/CloudKit_constants/index.html#//apple_ref/doc/constant_group/Record_Changed_Error_Keys>
  * Figure out how to resend the resolved items. I think you use CKRecordChangedErrorServerRecordKey (the CKRecord that currently exists on the server), make any field changes to it, save the changes locally to match, and then push these changes to the server. The trick is how to instrument this as part of the initial push of local changes
* Add handling for CKFetchRecordsOperation's moreComing feature
  * Somehow issue another CKFetchRecordsOperation (perhaps just call performSync: again?)
* Add code to automatically delete CKRecordZones that are not being used (in case the definition provided by BTCloudKitSyncDatabase changes)
* Add code to automatically delete unused CKSubscriptions (could happen if the developer changes what BTCloudKitSyncDatabase provides)
* Add an app icon for the BTCloudKitSyncDemo app

## Overview

BTCloudKitSync works through a **BTCloudKitSyncDatabase** protocol, which your app should implement. BTCloudKitSync doesn't care what technology you use for your local database. The demo uses SQLite, but you could use CoreData or something else.

In your BTCloudKitSyncDatabase implementation, you provide the types of records that should be synchronized. You also provide the changes as an array of NSDictionary objects, which are generally *insert*, *update*, or *delete* changes. In order to keep sync as efficient as possible, your change objects should only include the fields that actually changed.

Your BTCloudKitSyncDatabase also provides the names of notifications it posts via NSNotificationCenter. BTCloudKitSync observes these notifications and uses them to sync automatically if there has been no further changes after three seconds.

## Concepts used from CloudKit

1. CKModifyRecordsOperation to upload records to CloudKit
2. CKFetchChangesOperation to download record changes from CloudKit
3. CKSubscription for receiving changes from other devices
	* Sends silent Apple Push Notifications (APS) to the App Delegate when changes happen in the private CloudKit database from other devices

## The Demo App (BTCloudKitSyncDemo)

This is a simplistic address book app with a single [UITableViewController](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UITableViewController_Class/) that displays random first and last names.

* Tap on the **+** button to create a new record. The app will randomly generate a first and last name
* Tap on a contact row to randomly change the first/last name
* Swipe to delete a contact row
* The iOS Simulator cannot receive silent push notifications, but if you install the app on an iOS device and make changes in the simulator, you should see the iOS device update with the changes

The provided demo app uses SQLite for the local database. An important step in synchronizing data with CloudKit is tracking local changes. The demo project uses [SQLite Triggers](https://sqlite.org/lang_createtrigger.html) to track local changes, which makes tracking changes extremely simple.

When adding changes from the server, SQLite Triggers are temporarily disabled (no need to track server changes). [FMDatabaseQueue](https://ccgus.github.io/fmdb/html/Classes/FMDatabaseQueue.html) is used to perform queries and updates in the database.


## Acknowledgements
* CloudKit team at Apple - Thanks for the cool tech and also answering questions!

The demo app uses the following:

* FBDB - an Objective-C wrapper around SQLite: <https://github.com/ccgus/fmdb>
* SVProgressHUD - A lightweight progress HUD: <https://github.com/SVProgressHUD/SVProgressHUD>
