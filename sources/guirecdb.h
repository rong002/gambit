//
// guirecdb.h  -- a database class which is used in the gui
//                recording process
//

#ifndef GUIRECDB_H
#define GUIRECDB_H

#include "wx.h"
#include "wxmisc.h"

#include "gmisc.h" 
#include "gstream.h"  
#include "gtext.h"
#include "hash.h"


// Header files for objects to be logged:
#include "gambit.h"
#include "efgshow.h"
#include "spread.h"

#include <assert.h>

/*
 *
 * The GUI recorder database:
 *
 * Every time a recordable object is created, it registers itself with the
 * database.  It must tell the database three things:
 *
 * 1) Its name (a string);
 * 2) A pointer to itself (a void* pointer);
 * 3) A type ID (an int).
 *
 * These are stored in the database as a single GuiObject.  Later, during
 * playback, the name from the logfile is used to get the pointer to the
 * object and the type ID.  The pointer is cast to the appropriate type and
 * a callback function specific to that type is called, with the rest of
 * the line from the logfile as its arguments.  These are parsed to
 * determine what to do.
 *
 * The database MUST signal an error if it's asked to store an object with
 * the same name as one in the database.
 *
 */


// This is so other files can refer to the global 
// GuiRecorderDatabase object.

class GuiRecorderDatabase;
extern GuiRecorderDatabase gui_recorder_db;


// Macros; these make it easy to record objects into the database.

#define GUI_RECORDER_ADD_TO_DB(N, I, O) \
gui_recorder_db.Define(N, new GuiObject(I, O)); assert(gui_recorder_db.check());


// The Gui object type enumerators.
// FIXME: add to this as necessary.

enum GuiObjectTypeID
{
    GAMBIT_FRAME,
    EFG_SHOW,
    SPREADSHEET3D
};

#define FIRST_GUI_ID GAMBIT_FRAME
#define LAST_GUI_ID  SPREADSHEET3D


// The GuiObject class.

class GuiObject
{
private:
    GuiObjectTypeID   ID;
    void             *object;

public:
    // Constructor, destructor.
    GuiObject(GuiObjectTypeID i, void *o) 
    {
        ID = i;
        object = o;
    }

    virtual ~GuiObject() { }

    // Access functions.
    GuiObjectTypeID get_id() const { return ID;     }
    const void *get_object() const { return object; }

    // Debugging.
    bool is_GuiObject() const;
    void GuiObject::GuiObject_hello() const;
    bool check() const;
    void dump()  const;
};



// The GuiRecorderDatabase class.  This is just a hash table of
// GuiObject pointers.  It should only have one instance.

class GuiRecorderDatabase : public HashTable<gText, GuiObject*>
{
private:
    static bool instantiated;  // Only one instance is permitted.
    int  Hash(const gText& ref) const;
    void DeleteAction(GuiObject* object);

public:
    // ================== Exception classes. ====================

    class DatabaseInUse : public gException   
    {
    public:
        virtual ~DatabaseInUse()   { }
        gText Description() const;
    };
    
    // ==========================================================

    // Constructor, destructor.
    GuiRecorderDatabase();
    ~GuiRecorderDatabase();

    // Methods.
    int NumBuckets() const;

    // Debugging.
    bool is_GuiRecorderDatabase() const;
    void GuiRecorderDatabase::GuiRecorderDatabase_hello() const;
    bool check() const;
    void dump()  const;
};




#endif  // GUIRECDB_H



