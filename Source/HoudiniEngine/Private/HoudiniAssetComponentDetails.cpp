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

#include "HoudiniEnginePrivatePCH.h"


TSharedRef<IDetailCustomization>
FHoudiniAssetComponentDetails::MakeInstance()
{
	return MakeShareable(new FHoudiniAssetComponentDetails);
}


FHoudiniAssetComponentDetails::FHoudiniAssetComponentDetails()
{

}


void
FHoudiniAssetComponentDetails::CreateStaticMeshes(bool bMultipleMeshes)
{
	if(HoudiniAssetComponents.Num() > 0)
	{
		UHoudiniAssetComponent* HoudiniAssetComponent = HoudiniAssetComponents[0];
	
		TArray<UStaticMesh*> StaticMeshes;
		FHoudiniEngineUtils::CreateStaticMeshes(HoudiniAssetComponent->HoudiniAsset, HoudiniAssetComponent->HoudiniAssetObjectGeos,
												StaticMeshes, bMultipleMeshes);

		// Notify asset registry that we have created assets. This should update the content browser.
		for(int32 MeshIdx = 0; MeshIdx < StaticMeshes.Num(); ++MeshIdx)
		{
			FAssetRegistryModule::AssetCreated(StaticMeshes[MeshIdx]);
		}
	}
}


FReply
FHoudiniAssetComponentDetails::OnButtonClickedBakeSingle()
{
	CreateStaticMeshes(false);
	return FReply::Handled();
}


FReply
FHoudiniAssetComponentDetails::OnButtonClickedBakeMultiple()
{
	CreateStaticMeshes(true);
	return FReply::Handled();
}


void
FHoudiniAssetComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Get all components which are being customized.
	TArray<TWeakObjectPtr<UObject> > ObjectsCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsCustomized);

	// See if we need to enable baking option.
	for(int32 i = 0; i < ObjectsCustomized.Num(); ++i)
	{
		if(ObjectsCustomized[i].IsValid())
		{
			UObject* Object = ObjectsCustomized[i].Get();

			if(Object)
			{
				// We can't use Unreal cast here since we have patched RTTI for component.
				UHoudiniAssetComponent* HoudiniAssetComponent = (UHoudiniAssetComponent*) Object;
				HoudiniAssetComponents.Add(HoudiniAssetComponent);
			}
		}
	}

	// Create buttons for actions.
	DetailBuilder.EditCategory("HoudiniActions", TEXT(""), ECategoryPriority::Important )
		.AddCustomRow(TEXT(""))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(0, 2.0f, 0, 0)
			.FillHeight(1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2.0f, 0.0f)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(SButton)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.OnClicked(this, &FHoudiniAssetComponentDetails::OnButtonClickedBakeSingle)
						.Text(LOCTEXT("BakeHoudiniActor", "Bake Single"))
						.ToolTipText( LOCTEXT("BakeHoudiniActorToolTip", "Bake selected Houdini Actor into a single mesh"))
					]
				+SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2.0f, 0.0f)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(SButton)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.OnClicked(this, &FHoudiniAssetComponentDetails::OnButtonClickedBakeMultiple)
						.Text(LOCTEXT("BakeHoudiniActor", "Bake Multiple"))
						.ToolTipText( LOCTEXT("BakeHoudiniActorToolTip", "Bake selected Houdini Actor into multiple meshes"))
					]
			]
		];

	// Create Houdini Asset category.
	DetailBuilder.EditCategory("HoudiniAsset", TEXT(""), ECategoryPriority::Important);

	// Create Houdini Textures category.
	DetailBuilder.EditCategory("HoudiniTextures", TEXT(""), ECategoryPriority::Important);

	// Create Houdini Properties category.
	DetailBuilder.EditCategory("HoudiniProperties", TEXT(""), ECategoryPriority::Important);
}
