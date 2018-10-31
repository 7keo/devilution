
//----------------------------------------------------------------------------//

//HEADER_GOES_HERE
#ifndef __MAINMENU_H__
#define __MAINMENU_H__

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

extern char chr_name_str[16];

//----------------------------------------------------------------------------//

void __cdecl    mainmenu_cpp_init ();

//----------------------------------------------------------------------------//

void __cdecl    mainmenu_Refresh_Music ();
//----------------------------------------------------------------------------//

void __stdcall  mainmenu_Create_Hero (
  char* name_1,
  char* name_2
);
//----------------------------------------------------------------------------//

int  __stdcall  mainmenu_Select_Hero_Dialog (
  const struct _SNETPROGRAMDATA* u1,
  const struct _SNETPLAYERDATA*  u2,
  const struct _SNETUIDATA*      u3,
  const struct _SNETVERSIONDATA* u4,
        DWORD                    mode,   /// 4 chars, e.g. 'IPXN', 'BNET' etc.
        char*                    cname,  /// character name will be copied here
        DWORD                    clen,
        char*                    cdesc,  /// character "description" will be copied here (used to advertise games)
        DWORD                    cdlen,
        BOOL*                    multi   /// new character? - unsure about this
);

//----------------------------------------------------------------------------//

void __cdecl    mainmenu_Loop();

//----------------------------------------------------------------------------//

BOOL __cdecl    mainmenu_Single_Player ();

//----------------------------------------------------------------------------//

BOOL __fastcall mainmenu_Init_Menu ( int _type );

//----------------------------------------------------------------------------//

BOOL __cdecl    mainmenu_Multi_Player ();

//----------------------------------------------------------------------------//

void __cdecl    mainmenu_play_intro ();

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

/* rdata */

extern const int mainmenu_inf; // weak

//----------------------------------------------------------------------------//

/* data */

extern int menu_music_track_id; // idb

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

#endif /* __MAINMENU_H__ */

//----------------------------------------------------------------------------//