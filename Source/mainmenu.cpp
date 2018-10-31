
//----------------------------------------------------------------------------//

//HEADER_GOES_HERE

#include "../types.h"

//----------------------------------------------------------------------------//

static float mainmenu_cpp_init_value = INFINITY;
       char  chr_name_str[16];
/* data */
       int   menu_music_track_id = 5;

//----------------------------------------------------------------------------//

void __cdecl mainmenu_refresh_music()
{

	music_start (menu_music_track_id );

	do {

    menu_music_track_id += 1;

		if ( menu_music_track_id == 6 )
			menu_music_track_id = 0;

  } while ( menu_music_track_id == 0
    or      menu_music_track_id == 1 );

}

//----------------------------------------------------------------------------//

void __stdcall mainmenu_create_hero(
  char* _name_1,
  char* _name_2
){

	if ( not UiValidPlayerName( _name_1 ) )
    return;

  pfile_create_save_file( _name_1, _name_2 );

}

//----------------------------------------------------------------------------//

int __stdcall mainmenu_select_hero_dialog(
  const struct _SNETPROGRAMDATA* _u1,
  const struct _SNETPLAYERDATA*  _u2,
  const struct _SNETUIDATA*      _u3,
  const struct _SNETVERSIONDATA* _u4,
        DWORD                    _mode,  /// provider ?
        char*                    _player_name,
        DWORD                    _player_name_length,
        char*                    _player_description,
        DWORD                    _player_description_length,
        BOOL*                    _multi
){


	int selhero_is_created = 1;
	int dialog_result      = 0;

  //--------------------------------------------------------------------------//

	if ( gbMaxPlayers == 1 ){

		if ( not UiSelHeroSingDialog(
      pfile_ui_set_hero_infos,
      pfile_ui_save_create,
      pfile_delete_save,
      pfile_ui_set_class_stats,
      &dialog_result,
      chr_name_str,
      &gnDifficulty
    ) )
			TermMsg( "Unable to display SelHeroSing" );

		if  ( dialog_result == 2 )
      dword_5256E8 = TRUE;
		else
      dword_5256E8 = FALSE;

	}

  else
  if ( not UiSelHeroMultDialog(
    pfile_ui_set_hero_infos,
	  pfile_ui_save_create,
	  pfile_delete_save,
	  pfile_ui_set_class_stats,
	  &dialog_result,
	  &selhero_is_created,
	  chr_name_str
  ) )
		TermMsg( "Can't load multiplayer dialog" );

  //--------------------------------------------------------------------------//

	if ( dialog_result == 4 ){
		SErrSetLastError( 1223 );
		return 0;
	}

  //--------------------------------------------------------------------------//

	pfile_create_player_description(
    _player_description,
    _player_description_length
  );

  //--------------------------------------------------------------------------//

	if ( _multi != NULL ){

		if ( _mode == 'BNET' )
			*_multi = ( selhero_is_created or not plr[myplr].pBattleNet );
		else
			*_multi = selhero_is_created;

	}

  //--------------------------------------------------------------------------//

	if ( _player_name        != NULL
  and  _player_name_length != 0 )
		SStrCopy(
      _player_name,
      chr_name_str,
      _player_name_length
    );

  //--------------------------------------------------------------------------//

	return 1;

}

//----------------------------------------------------------------------------//

void __cdecl mainmenu_loop (){


	mainmenu_refresh_music();


  int game_mode_running;
  do {

		while ( true ){

			int dialog_result = 0;
			if ( not UiMainMenuDialog(
        gszProductName,
        &dialog_result,
        effects_play_sound,
        30 )  /// magic number
      )
				TermMsg( "Unable to display mainmenu" );

      switch ( dialog_result ){


        case __mainmenu_single_player:
          goto Label_Single_Player;


        case __mainmenu_multiplayer:
          game_mode_running = mainmenu_Multi_Player();
          goto Label_Skip;


        case __mainmenu_replay_intro:
          goto Label_Play_Intro;


        case __mainmenu_show_credits:
          UiCreditsDialog( 16 );  /// magic number
          break;


        case __mainmenu_exit_diablo:
          goto Label_Exit;


        case __mainmenu_attract_mode:
          Label_Play_Intro:
          if ( gbActive )
            mainmenu_play_intro();
          break;


			}
		}

		Label_Single_Player:
    game_mode_running = mainmenu_Single_Player();

    Label_Skip:;

	} while ( game_mode_running );

  Label_Exit:
	music_stop();

}
// 634980: using guessed type int gbActive;

//----------------------------------------------------------------------------//

BOOL __cdecl mainmenu_Single_Player()
{
	gbMaxPlayers = 1;
	return mainmenu_init_menu(1);
}
// 679660: using guessed type char gbMaxPlayers;

BOOL __fastcall mainmenu_init_menu(int type)
{
	if (type == 4)
		return 1;

	music_stop();

	int success = diablo_init_menu(type != 2, type != 3);
	if (success)
		mainmenu_refresh_music();

	return success;
}

BOOL __cdecl mainmenu_Multi_Player()
{
	gbMaxPlayers = MAX_PLRS;
	return mainmenu_init_menu(3);
}

void __cdecl mainmenu_play_intro()
{
	music_stop();
	play_movie("gendata\\diablo1.smk", 1);
	mainmenu_refresh_music();
}
