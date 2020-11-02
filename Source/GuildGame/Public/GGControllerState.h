// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GUILDGAME_API GGControllerState
{
    public:
    virtual void Update() = 0;
    virtual void LeftClickHandler() = 0;
    virtual void LeftClickReleaseHandler() = 0;
    virtual void RightClickHandler() = 0;
    virtual void RightClickReleaseHandler() = 0;
    virtual void ESCHandler() = 0;

    virtual void ChangeTo() = 0;
    virtual void ChangeFrom() = 0;
    virtual bool CanChangeTo() = 0;
    virtual ~GGControllerState() = default;
};

class GUILDGAME_API ControllerStateDefault: public GGControllerState
{
    public:
    explicit ControllerStateDefault(class AGGPlayerController*);
    virtual ~ControllerStateDefault() = default;
    void Update() override;
    void LeftClickHandler() override;
    void LeftClickReleaseHandler() override;
    void RightClickHandler() override;
    void RightClickReleaseHandler() override;
    void ESCHandler() override;
    void ChangeTo() override;
    void ChangeFrom() override;
    bool CanChangeTo() override;

    private:
    class AGGPlayerController* PlayerController;
};

class GUILDGAME_API ControllerStateBasicAttack: public GGControllerState
{
    public:
    explicit ControllerStateBasicAttack(class AGGPlayerController*);
    virtual ~ControllerStateBasicAttack() = default;
    void Update() override;
    void LeftClickHandler() override;
    void LeftClickReleaseHandler() override;
    void RightClickHandler() override;
    void RightClickReleaseHandler() override;
    void ESCHandler() override;
    void ChangeTo() override;
    void ChangeFrom() override;
     bool CanChangeTo() override;

    private:
    class AGGPlayerController* PlayerController;
};

