/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Produced by:
 *      Mykola Konyk
 *      Side Effects Software Inc
 *      123 Front Street West, Suite 1401
 *      Toronto, Ontario
 *      Canada   M5J 2M2
 *      416-504-9876
 *
 */

#pragma once
#if 0
class IHoudiniTaskCookAssetCallback;
struct FHoudiniEngineNotificationInfo;

class FHoudiniTaskCookAsset : public FRunnable
{
public:

	FHoudiniTaskCookAsset(IHoudiniTaskCookAssetCallback* InHoudiniTaskCookAssetCallback, UHoudiniAssetInstance* InHoudiniAssetInstance);
	virtual ~FHoudiniTaskCookAsset();

public: /** FRunnable methods. **/

	virtual uint32 Run() OVERRIDE;

private:

	/** Helper function used to clean up while running in case of error. **/
	uint32 RunErrorCleanUp(HAPI_Result Result);

	/** Instruct engine to remove notification used by this runnable. **/
	void RemoveNotification();

	/** Update notification with new status string. **/
	void UpdateNotification(const FString& StatusString);

protected:

	/** Notification used by this runnable. **/
	TSharedPtr<FHoudiniEngineNotificationInfo> NotificationInfo;

	/** Callback for completion / status report. **/
	IHoudiniTaskCookAssetCallback* HoudiniTaskCookAssetCallback;

	/** Houdini asset instance we are cooking. **/
	UHoudiniAssetInstance* HoudiniAssetInstance;

	/** Tracks the last time notification has been used to avoid spamming. **/
	double LastUpdateTime;
};
#endif
