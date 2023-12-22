// Copyright JYJ


#include "Game/AuraGameModeBase.h"


// Sets default values
AAuraGameModeBase::AAuraGameModeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAuraGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

