#include "client.hpp"
#include "offsets.h"

namespace col
{
	FLinearColor white  = FLinearColor(1, 1, 1, 1);
	FLinearColor black  = FLinearColor(0, 0, 0, 1);
	FLinearColor red    = FLinearColor(0, 0, 0, 1);
	FLinearColor green  = FLinearColor(0, 0, 0, 1);
	FLinearColor blue   = FLinearColor(0, 0, 0, 1);
	FLinearColor orange = FLinearColor(0, 0, 0, 1);
	FLinearColor lime   = FLinearColor(0, 0, 0, 1);
	FLinearColor gray   = FLinearColor(0, 0, 0, 1);
}

std::vector<std::pair<float, AActor*>> actors_distance_array;

// The function will return 1 if pair2 > pair1 else 0
bool compare_pair(const std::pair<float, AActor*>& pair1, const std::pair<float, AActor*>& pair2)
{
	return pair1.first < pair2.first;
}

void client::tick(UWorld* uworld_addr)
{
	actors_distance_array.clear();

	frame_counter++;
	if (frame_counter == 120)
	{
		GetClientRect(FindWindowA(skCrypt("UnrealWindow"), skCrypt("ARK: Survival Evolved")), &window_rect);
		frame_counter = 0;
	}

	UWorld* gworld = *reinterpret_cast<decltype(UWorld::GWorld)*>(uworld_addr);

	if (!gworld
		|| !gworld->GameState
		|| !gworld->OwningGameInstance
		|| !gworld->OwningGameInstance->LocalPlayers[0]
		|| !gworld->OwningGameInstance->LocalPlayers[0]->PlayerController
		|| !gworld->OwningGameInstance->LocalPlayers[0]->PlayerController->AcknowledgedPawn)
		return;

	ULevel*              persistent_level = gworld->PersistentLevel;
	TTransArray<AActor*> actors_array     = persistent_level->Actors;
	UGameInstance*       game_instance    = gworld->OwningGameInstance;
	UPlayer*             local_uplayer    = game_instance->LocalPlayers[0];
	APlayerController*   local_controller = game_instance->LocalPlayers[0]->PlayerController;
	AActor*              local_player     = game_instance->LocalPlayers[0]->PlayerController->AcknowledgedPawn;


	/* If we're not in a game world don't bother checking anything else, actors count is fine for this... */
	if (actors_array.Count < 50)
	{
		draw_menu();
		return;
	}

	FVector local_player_location = local_player->k2_get_actor_location();

	for (int i = 0; i < actors_array.Count; i++)
	{
		AActor*   actor                = actors_array[i];

		if (!actor)
			continue;

		FVector	  actor_world_location = actor->k2_get_actor_location();
		FVector2D actor_screen_location;

		if (!actor_world_location.X || !actor_world_location.Y || !actor_world_location.Z)
			continue;
		if (!w2s(actor_world_location, actor_screen_location, local_controller, window_rect))
			continue;

		bool player    = actor->is_player();
		bool dino      = actor->is_dino();
		bool structure = actor->is_structure();

		if (!player && !dino && !structure)
			continue;

		FLinearColor actor_color = FLinearColor(1, 1, 1, 1); // default to white

		if (player || dino)
		{
			bool friendly  = actor->is_primal_char_friendly((APrimalCharacter*)local_player);
			bool conscious = actor->is_conscious();
			bool dead      = actor->is_dead();
	
			uintptr_t status_component = (*(uintptr_t*)(((uintptr_t)actor)          + Offsets::Classes::APrimalCharacter::MyCharacterStatusComponent));
			int       base_level       = (*(int32_t*)(((uintptr_t)status_component) + Offsets::Classes::UPrimalCharacterStatusComponent::BaseCharacterLevel));
			int       extra_evel       = (*(int32_t*)(((uintptr_t)status_component) + Offsets::Classes::UPrimalCharacterStatusComponent::ExtraCharacterLevel));
			
			float current_health = (*(float*)(((uintptr_t)actor) + Offsets::Classes::APrimalCharacter::ReplicatedCurrentHealth));
			float max_health     = (*(float*)(((uintptr_t)actor) + Offsets::Classes::APrimalCharacter::ReplicatedMaxHealth));
			float current_torpor = (*(float*)(((uintptr_t)actor) + Offsets::Classes::APrimalCharacter::ReplicatedCurrentTorpor));
			float max_torpor     = (*(float*)(((uintptr_t)actor) + Offsets::Classes::APrimalCharacter::ReplicatedMaxTorpor));


			if (player)
			{
				actor_color = friendly  ? col::blue : col::red;
				actor_color = conscious ? col::lime : actor_color;
				actor_color = dead      ? col::gray : actor_color;

				if (!settings.player.friendly && friendly)   continue;
				if (!settings.player.enemy    && !friendly)  continue;
				if (!settings.player.sleeping && !conscious) continue;
				if (!settings.player.dead     && !dead)      continue;

				

			}
			if (dino)
			{
				bool tamed = actor->is_tamed();

				actor_color = friendly  ? col::blue : tamed ? col::orange : col::green;
				actor_color = conscious ? col::lime : actor_color;
				actor_color = dead      ? col::gray : actor_color;

				if (!settings.dino.friendly && friendly)   continue;
				if (!settings.dino.enemy    && !friendly)  continue;
				if (!settings.dino.wild     && !tamed)     continue;
				if (!settings.dino.dead     && !dead)      continue;


			}
		}
	}

}


void client::draw_text(const std::string& text, const FVector2D& location, const FLinearColor& color)
{
	temporary_wstring.assign(text.begin(), text.end());
	return canvas->k2_draw_text(get_font(), FString(temporary_wstring.c_str()), location, color, 1, FLinearColor(0, 0, 0, 1), FVector2D(0, 0), true, true, true, FLinearColor(0, 0, 0, 1));
}

void client::draw_menu()
{

}