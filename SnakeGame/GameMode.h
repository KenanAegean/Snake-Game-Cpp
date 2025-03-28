// GameMode.h
#pragma once

enum class PlayMode {
    OnePlayer,
    TwoPlayerVersus,
    TwoPlayerCooperative,
    PlayerVsAI,
    PlayerAndAI
};

struct GameModeSettings {
    PlayMode mode;
    int targetScore;
};