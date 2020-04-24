#include "interpreter.h"
#include "leafedit_api.h"

void UnixSetupFunc() {}

/* list of all library functions and their prototypes */
struct LibraryFunction UnixFunctions[] =
{
	// Msg.
	{ msg_warn,				"void msg_warn(char* warning);" },
	{ msg_waitMsg,			"void msg_waitMsg(char* message);" },
	{ msg_splash,			"void msg_splash(char* notification);" },
	{ msg_prompt, 			"int msg_prompt(char* message);" },
	// SaveUtils - Read.
	{ sav_read_u8, 			"unsigned int sav_read_u8(unsigned int offset);" },
	{ sav_read_u16, 		"unsigned int sav_read_u16(unsigned int offset);" },
	{ sav_read_u32, 		"unsigned int sav_read_u32(unsigned int offset);" },
	// SaveUtils - Write.
	{ sav_write_u8, 		"void sav_write_u8(unsigned int offset, unsigned int value);" },
	{ sav_write_u16, 		"void sav_write_u16(unsigned int offset, unsigned int value);" },
	{ sav_write_u32, 		"void sav_write_u32(unsigned int offset, unsigned int value);" },
	// String Getter.
	{ getItem,				"char *getItem(unsigned int ID);"},
	// Keyboard.
	{ keyboard_string,		"char *keyboard_string(char* message);"},
	{ keyboard_value,		"int keyboard_value(char* message);"},
	// List Selections.
	{ selectList,			"int selectList(char* message, char** contents, int options);"},
	// Misc.
	{ setChangesMade,		"void setChangesMade();"},
	{ download_file,		"void download_file(char* URL, char* Path, char* Message);"}, 
	{ file_select,			"char* file_select(char* Path, char* Text);"},
	// End.
	{ NULL,					NULL }
};

void PlatformLibraryInit(Picoc *pc)
{
	IncludeRegister(pc, "leafedit.h", &UnixSetupFunc, &UnixFunctions[0], "");
}
