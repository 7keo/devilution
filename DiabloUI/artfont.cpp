
//----------------------------------------------------------------------------//

// ref: 0x10001058
void __fastcall Font_Set_Current (
  int nFont
){

  //// should rearrange fonts into an array and do the following:
  //
  // fonts [] = {
  //   &font16g,
  //   &font16s,
  //   &font24g,
  //   &font24s,
  //   &font30s,
  //   &font42g,
  //   &font42y
  //  };
  //
  // if ( n_font < 0
  // or   n_font > 7 )
  //   n_fonts = 1;

  // current_font = fonts[n_font];



	switch ( nFont ){

    case 0:
      sgpCurrFont = &font16g;
      break;

    case 2:
      sgpCurrFont = &font24g;
      break;

    case 3:
      sgpCurrFont = &font24s;
      break;

    case 4:
      sgpCurrFont = &font30g;
      break;

    case 5:
      sgpCurrFont = &font30s;
      break;

    case 6:
      sgpCurrFont = &font42g;
      break;

    case 7:
      sgpCurrFont = &font42y;
      break;

    default:
      sgpCurrFont = &font16s;
      break;

	}
}

//----------------------------------------------------------------------------//

// ref: 0x10001098
void __cdecl Font_Init_All (){

	font42g.active = false;
	font42y.active = false;
	font30g.active = false;
	font30s.active = false;
	font24g.active = false;
	font24s.active = false;
	font16g.active = false;
	font16s.active = false;

	sgpCurrFont    = NULL;

}

//----------------------------------------------------------------------------//

BOOL inline Current_Font_Ready (){

  return sgpCurrFont != NULL
     and sgpCurrFont->active;

}

//----------------------------------------------------------------------------//

BOOL inline Draw_Surface_Ready (
  DRAW_SURFACE* _draw_surface
){

  return _draw_surface         != NULL
     and _draw_surface->pixels != NULL;

}

//----------------------------------------------------------------------------//

void inline Deallocate_Glyph (
  HANDLE* _glyph_texture_handle
){

  if ( *_glyph_texture_handle == NULL )
    return;

  STransDelete( *_glyph_texture_handle );

  (*_glyph_texture_handle) = NULL;

}

//----------------------------------------------------------------------------//

// ref: 0x10001120
void __fastcall Font_Deallocate (
  FONT* _pFont
){

	if ( _pFont->active == false )
    return;

  for ( int n=false; n<256; n+=1 )  /// magic number 256: font num glyphs
    Deallocate_Glyph( &(_pFont->fonttrans[n]) );

  _pFont->active = false;

}

//----------------------------------------------------------------------------//

// ref: 0x100010C8
void __cdecl Font_Deallocate_All (){

	Font_Deallocate( &font42g );
	Font_Deallocate( &font42y );
	Font_Deallocate( &font30g );
	Font_Deallocate( &font30s );
	Font_Deallocate( &font24g );
	Font_Deallocate( &font24s );
	Font_Deallocate( &font16g );
	Font_Deallocate( &font16s );

	sgpCurrFont = NULL;

}

//----------------------------------------------------------------------------//

// ref: 0x10001159
BOOL __cdecl Font_Load_All (){

	Font_Load_From_File( &font30g, "ui_art\\font30.bin", "ui_art\\font30g.pcx" );
	Font_Load_From_File( &font30s, "ui_art\\font30.bin", "ui_art\\font30s.pcx" );
	Font_Load_From_File( &font24g, "ui_art\\font24.bin", "ui_art\\font24g.pcx" );
	Font_Load_From_File( &font24s, "ui_art\\font24.bin", "ui_art\\font24s.pcx" );
	Font_Load_From_File( &font16g, "ui_art\\font16.bin", "ui_art\\font16g.pcx" );
	Font_Load_From_File( &font16s, "ui_art\\font16.bin", "ui_art\\font16s.pcx" );
	Font_Load_From_File( &font42g, "ui_art\\font42.bin", "ui_art\\font42g.pcx" );
	Font_Load_From_File( &font42y, "ui_art\\font42.bin", "ui_art\\font42y.pcx" );

	return true;

}

//----------------------------------------------------------------------------//

void inline Font_Clear_Glyph_Texture_Handles (
  FONT* _font
){

  memset( _font->fonttrans, NULL, sizeof(HANDLE)*256 );

}

//----------------------------------------------------------------------------//

BOOL inline Font_Load_Data (
  FONT*   _font,
  CSTRING _filename
){


	if ( _font->active )
    return false;


  HANDLE file;

  if ( not SFileOpenFile( _filename, &file ) )
    return false;


  BOOL status = SFileReadFile(
    file,
    _font,
    SFileGetFileSize( file, NULL ),
    NULL,    /// dont store number of bytes read
    NULL );  /// no asynchronous reading

  SFileCloseFile( file );

  return status;

}

//----------------------------------------------------------------------------//

void inline Font_Create_Glyph_Texture (
  FONT*  _font,
  BYTE   _glyph,
  BYTE*  _buffer,
  _SIZE* _size,
  DWORD  _buffer_size
){

  const BYTE glyph_width = _font->glyph_widths[ _glyph ];
  if ( glyph_width == 0 )
    return;

  RECTI source_rect;
    source_rect.left   = 0;
    source_rect.top    = ( _glyph * _font->line_height );
    source_rect.right  = glyph_width;
    source_rect.bottom = ( source_rect.top + _font->line_height - 1 );

  STransCreateI(
    _buffer,
    _size->w,
    _size->h,
    BITS_PER_PIXEL_8,
    (_RECT*)&source_rect,
    _buffer_size,
    (HANDLE*)&(_font->fonttrans[ _glyph ]) );

}

//----------------------------------------------------------------------------//

// ref: 0x100011FB
void __fastcall Font_Load_From_File (
  FONT*   _font,
	CSTRING _data_filename,
	CSTRING _image_filename
){


  if ( not Font_Load_Data( _font, _data_filename ) )
    return;


  Font_Clear_Glyph_Texture_Handles( _font );

  BYTE* buffer;
  _SIZE size;
  const DWORD buffer_size = 0x01000020;

  local_LoadArtImage( _image_filename, &buffer, &size );

  if ( buffer == NULL )
    return;


  for ( WORD n=0; n<256; n+=1 )
    Font_Create_Glyph_Texture( _font, n, buffer, &size, buffer_size );

  _font->active = true;

  char* const this_file = "C:\\Src\\Diablo\\DiabloUI\\artfont.cpp";
  const DWORD this_line = 206;
  SMemFree( buffer, this_file, this_line, 0x00 );

}

//----------------------------------------------------------------------------//

// ref: 0x100012F6
int __cdecl Font_Line_Height (){   /// should be inline

	if ( not Current_Font_Ready() )
    return 0;

  return sgpCurrFont->line_height;

}

//----------------------------------------------------------------------------//

// ref: 0x10001310
int __cdecl Font_Default_Width (){  /// should be inline

	if ( not Current_Font_Ready() )
    return 0;

	return sgpCurrFont->default_width;

}

//----------------------------------------------------------------------------//

// ref: 0x10001329
int __fastcall Font_Calc_Line_Width (
  CSTRING _str
){  /// might want to return unsigned


  if ( not Current_Font_Ready() )
    return false;


  int total_width = 0;

	for (
    DWORD n = 0;
    _str[n] != '\0';
    n += 1
  ){

		BYTE glyph_width = sgpCurrFont->glyph_widths[ n ];

    if ( glyph_width == false )
			glyph_width = sgpCurrFont->default_width;

		total_width += glyph_width;

	}

	return total_width;

}

//----------------------------------------------------------------------------//

// ref: 0x1000136C
void __cdecl Font_Init (){  /// should be inline

	artfont_cpp_float = INFINITY; // 0x7F80'0000;

}

//----------------------------------------------------------------------------//

// 10026BB0: using guessed type int artfont_cpp_float;

// ref: 0x10001377
int __fastcall Font_Find_Linebreak_Width (
  CSTRING _str
){  /// might want to return unsigned


	if ( not Current_Font_Ready() )
		return 0;


  int  line_width = false;
	WORD n      = false;
  while ( true ){  // for ( int n=false, BYTE glyph=_str[n];  glyph!='false';  n+=1, glyph=_str[n]  )

		const BYTE glyph = _str[n];
		if ( glyph == '\0'
    or   glyph == '\n'
		or   glyph == ' ' )
			return line_width;

		const BYTE glyph_width = sgpCurrFont->glyph_widths[ glyph ];
		if ( glyph_width == 0 )
			return line_width;

		line_width += glyph_width;
		n          += 1;

	}

}

//----------------------------------------------------------------------------//

// ref: 0x100013B3
void __cdecl Font_Delete_Operator (
  HANDLE _ptr
){

	if ( _ptr == NULL )
    return;

  char* const this_file = "delete";
  const int   this_line = -1;
  SMemFree( _ptr, this_file, this_line, 0x00 );

}

//----------------------------------------------------------------------------//

// ref: 0x100013CD
/// i think this is only used to print single line big font on UI
void __fastcall Font_Print_Text (
  CSTRING       _text,
  DRAW_SURFACE* _draw_surface,
  int           _x,
  int           _y
){


	if ( not Draw_Surface_Ready( _draw_surface )
  or   not Current_Font_Ready() )
    return;


  /// TO DO:
  /// exit early if text is empty
  /// exit early if text bottom is lower than draw surface bottom

  const BYTE default_width  = sgpCurrFont->default_width;
  const BYTE line_height    = sgpCurrFont->line_height;
  const int  surface_width  = _draw_surface->width;
  const int  surface_height = _draw_surface->height;

  DWORD out_x = ( _x >= 0 ) ? _x : 0;
  DWORD out_y = ( _y >= 0 ) ? _y : 0;

  BOOL new_line = false;

  DWORD n = 0;

  // for (  DWORD n=0;  _text[n]!='\0';  n+=1 ){
  while ( true ){

    //------------------------------------------------------------------------//

    const BYTE glyph = _text[n];

    if ( glyph == '\0' )
      return;

    //------------------------------------------------------------------------//

    if ( new_line ){
      /// out_x    = 0;  /// original value, but why?
      out_x    = _x;
      out_y   += line_height;
      new_line = false;
    }

    //------------------------------------------------------------------------//

    const DWORD bottom = out_y
                       + line_height;

    if ( bottom > surface_height )
      return;

    //------------------------------------------------------------------------//

    if ( glyph == '\n' ){

      /// do newline
      new_line = true;
      n       += 1;

      continue;

    }

    //------------------------------------------------------------------------//

    const BYTE glyph_width = sgpCurrFont->glyph_widths[ glyph ];

    /// TODO: figure out why it does this
    if ( glyph_width == 0 ){

      out_x += default_width;
      n     += 1;

      const DWORD far_right = out_x
                            + Font_Find_Linebreak_Width( &(_text[n]) );

      if ( far_right >= surface_width )
        new_line = true;

      continue;

    }

    //------------------------------------------------------------------------//

    HANDLE glyph_texture_handle = sgpCurrFont->fonttrans[ glyph ];

    /// by default this only happens if the font is not initialized
    /// and results in an endless loop
    if ( glyph_texture_handle == NULL )
      continue;

    //------------------------------------------------------------------------//

    const DWORD right = out_x
                      + glyph_width;

    if ( right > surface_width ){
      new_line = true;
      /// we dont want to advance character yet
      /// only wrap text to new line
      /// since we still have to print the current chacater
      continue;
    }

    //------------------------------------------------------------------------//

    STransBlt(  /// draw glyph ( glyp, x, y, surface )
      (HANDLE)_draw_surface->pixels,
      out_x,
      out_y,
      surface_width,
      glyph_texture_handle
    );

    //------------------------------------------------------------------------//

    out_x += glyph_width;
    n     += 1;

    //------------------------------------------------------------------------//

  }

}

//----------------------------------------------------------------------------//
