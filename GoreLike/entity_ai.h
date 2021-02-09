#pragma once

struct ai_wander {
	size_t distance = 2147000000;
};

//AI_WANDER but can bump into players aka most of the time hurt them?
struct ai_bump_and_wander {
	size_t distance = 2147000000;
};

//components for AI to decide which attack to use if both are present make it based on preferred range for attack
//	for whatever they are attacking with
struct ai_range_attack {};
struct ai_melee_attack {};

struct ai_chase_player {
	//The distance the AI tries to reach from the player
	unsigned short ai_distance = 1;
};

