// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopUI.h"
#include "../Actors/TowerBase.h"

void UShopUI::InitShop(AShop* shopRef)
{
	ShopOwner = shopRef;
}

void UShopUI::UpdateTowerInstiagator(ATowerBase* initiator)
{
	TowerBaseInitiator = initiator;
}
