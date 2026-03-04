// Fill out your copyright notice in the Description page of Project Settings.


#include "PFVolume.h"

#include "PFVolumeDebugComponent.h"

APFVolume::APFVolume()
{
    UPFVolumeDebugComponent* DebugComp = CreateDefaultSubobject<UPFVolumeDebugComponent>(TEXT("DebugComponent"));
}
