// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GUILDGAME_API BattleControllerState
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
    virtual ~BattleControllerState() = default;
};

class GUILDGAME_API ControllerStateDefault: public BattleControllerState
{
    public:
    explicit ControllerStateDefault(class ABattlePlayerController*);
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
    class ABattlePlayerController* PlayerController;
};

class GUILDGAME_API ControllerStateBasicAttack: public BattleControllerState
{
    public:
    explicit ControllerStateBasicAttack(class ABattlePlayerController*);
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
    class ABattlePlayerController* PlayerController;
};

