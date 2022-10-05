// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <Components\SceneComponent.h>
#include "Components/SphereComponent.h"
#include "DamageTaker.h"
#include "GameStructs.h"
#include "Particles\ParticleSystem.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles\ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SceneComp);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SphereCollision);
}


void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, MoveRate);
	GetWorld()->GetTimerManager().SetTimer(DeactivateTimer, this, &AProjectile::Deactivate, DeactivateTime, false);
}

void AProjectile::Deactivate()
{
	bIsActivation = false;
	SetActorLocation(FVector(0.0f, 0.0f, -50.0f));
	GetWorld()->GetTimerManager().ClearTimer(DeactivateTimer);
	GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
	SetActorEnableCollision(false);
}

void AProjectile::Move()
{
	FVector movePosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(movePosition);
}

void AProjectile::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
		if (DamageTakerActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = GetOwner();
			damageData.DamageMaker = this;

			DamageTakerActor->TakeDamage(damageData);
		}
		else
		{
			OtherActor->Destroy();
		}
	}
	Template = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation());
	Deactivate();
	//Destroy();
}

