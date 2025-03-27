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
    int targetScore; // For competitive modes, game ends when one player's score >= targetScore.
    // For cooperative modes, game ends when total score >= targetScore.
};
