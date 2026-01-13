#pragma once

#include "CoreMinimal.h"
#include "CT_CameraModeSettings.generated.h"

USTRUCT(BlueprintType)
struct FCT_CameraModeSettings
{
	GENERATED_BODY()

	// Spring arm / framing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ArmLength = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector SocketOffset = FVector::ZeroVector;

	// Lag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Lag")
	bool bEnableCameraLag = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Lag", meta = (EditCondition = "bEnableCameraLag", ClampMin = "0.0"))
	float CameraLagSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Lag")
	bool bEnableCameraRotationLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Lag", meta = (EditCondition = "bEnableCameraRotationLag", ClampMin = "0.0"))
	float CameraRotationLagSpeed = 10.f;

	// Lens
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Lens", meta = (ClampMin = "5.0", ClampMax = "170.0"))
	float FOV = 90.f;

	// Policy toggles (Phase 0.5)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Modifiers")
	bool bAllowSprintModifier = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Modifiers")
	bool bAllowCrouchModifier = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Modifiers")
	bool bAllowInAirModifier = true;

	// Optional: if you want a subtle shoulder bias per mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector TargetOffset = FVector::ZeroVector; // used later if you implement focus aiming
};
