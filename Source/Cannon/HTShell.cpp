// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "HTShell.h"





AHTShell::AHTShell() : AShell::AShell()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShelllObject(TEXT("/Game/balaHT")); // wherein /Game/ is the Content folder.
	Shell->SetStaticMesh(ShelllObject.Object);
}
