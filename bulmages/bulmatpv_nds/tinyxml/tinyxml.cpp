/*
 * Last revision:  Wed 22-Jul-2009 23:46 bushbo 
 * Version:  0.1
 * Copyright (c) 2009 by Brian O. Bush, http://kd7yhr.org
 * License: GNU GPL, version 3 or later; http://www.gnu.org/copyleft/gpl.html
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tinyxml.h"

/* Tables and constants */

/* Types of characters; 0 is not valid, 1 is letters, 2 are digits
   (including '.') and 3 whitespace. Also generated with a throw-away
   python script. */
static int char_type[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* Internal states that the parser can be in at any given time. */
enum {
  ST_START = 0,         /* starting base state, default state */
  ST_TEXT,              /* text state */
  ST_START_TAG,         /* start tag state */
  ST_START_TAGNAME,     /* start tagname state */
  ST_START_TAGNAME_END, /* start tagname ending state */
  ST_END_TAG,           /* end tag state */
  ST_END_TAGNAME,       /* end tag tagname state */
  ST_END_TAGNAME_END,   /* end tag tagname ending */ 
  ST_EMPTY_TAG,         /* empty tag state */
  ST_SPACE,             /* linear whitespace state */
  ST_ATTR_NAME,         /* attribute name state */
  ST_ATTR_NAME_END,     /* attribute name ending state */
  ST_ATTR_VAL,          /* attribute value starting state */
  ST_ATTR_VAL2,         /* attribute value state */
  ST_ERROR              /* error state */
};

/* character classes that we will match against; This could be expanded if
   need be, however, we are aiming for simple. */
enum {
  CLAZZ_NONE = 0,       /* matches nothing, a base state */
  CLAZZ_LEFT_ANGLE,     /* matches start tag '<' */
  CLAZZ_SLASH,          /* matches forward slash */
  CLAZZ_RIGHT_ANGLE,    /* matches end tag '>' */
  CLAZZ_EQUALS,         /* matches equals sign */
  CLAZZ_QUOTE,          /* matches double-quotes */
  CLAZZ_LETTERS,        /* matches a-zA-Z letters and digits 0-9 */
  CLAZZ_SPACE,          /* matches whitespace */
  CLAZZ_ANY             /* matches any ASCII character; will match all
                           above classes */
};

/* State transition table element; contains (1) current state, (2)
   clazz that must match, (3) next state if we match, and (4) event
   that is emitted upon match. If you want to see how take a simple
   idea and make it overly complicated, see:
   http://www.faqs.org/rfcs/rfc3076.html and
   http://www.w3.org/TR/REC-xml/ for details on XML.
*/
typedef struct state_t {
  int state, clazz, next_state, event;
} state_t;

/* Note: States must be grouped in match order AND grouped together! */
static state_t TINYXML_STATES [] = {
  /* [0-2] starting state, which also serves as the default state in case
     of error */
  { ST_START,         CLAZZ_SPACE,        ST_SPACE,             EVENT_NONE },
  { ST_START,         CLAZZ_LEFT_ANGLE,   ST_START_TAG,         EVENT_NONE },
  { ST_START,         CLAZZ_ANY,          ST_TEXT,              EVENT_COPY },

  /* [3-5] space state handles linear white space */
  { ST_SPACE,         CLAZZ_SPACE,        ST_SPACE,             EVENT_NONE },
  { ST_SPACE,         CLAZZ_LEFT_ANGLE,   ST_START_TAG,         EVENT_TEXT },
  { ST_SPACE,         CLAZZ_ANY,          ST_TEXT,              EVENT_COPY },

  /* [6-8] handle start tag */
  { ST_START_TAG,     CLAZZ_LETTERS,      ST_START_TAGNAME,     EVENT_COPY },
  { ST_START_TAG,     CLAZZ_SLASH,        ST_END_TAG,           EVENT_COPY },
  /* below added since some individuals get a little carried away with
     spacing around tag names, e.g. < tag > */
  { ST_START_TAG,     CLAZZ_SPACE,        ST_START_TAG,         EVENT_NONE },

  /* [9-12] handle start tag name */
  { ST_START_TAGNAME, CLAZZ_LETTERS,      ST_START_TAGNAME,     EVENT_NONE },
  { ST_START_TAGNAME, CLAZZ_SPACE,        ST_START_TAGNAME_END, EVENT_START },
  /* below added for tags without any space between tag and ending
     slash, e.g., <br/> */     
  { ST_START_TAGNAME, CLAZZ_SLASH,        ST_EMPTY_TAG,         EVENT_END },
  { ST_START_TAGNAME, CLAZZ_RIGHT_ANGLE,  ST_START,             EVENT_START },

  /* [13-16] handle start tag name end */
  { ST_START_TAGNAME_END,  CLAZZ_LETTERS, ST_ATTR_NAME,         EVENT_COPY },
  /* below added to handle additional space in between attribute value
     pairs in start tags, e.g., <tag attr="2" attr2="test" > */
  { ST_START_TAGNAME_END,  CLAZZ_SPACE,   ST_START_TAGNAME_END, EVENT_NONE },
  { ST_START_TAGNAME_END,  CLAZZ_RIGHT_ANGLE, ST_START,         EVENT_START },
  /* below supports tags that are self-closing, e.g., <br /> */
  { ST_START_TAGNAME_END,  CLAZZ_SLASH,   ST_EMPTY_TAG,         EVENT_COPY }, 

  /* [17] handle empty tags, e.g., <br /> */
  { ST_EMPTY_TAG,     CLAZZ_RIGHT_ANGLE,  ST_START,             EVENT_END },

  /* [18] handle end tag, e.g., <tag /> */
  { ST_END_TAG,       CLAZZ_LETTERS,      ST_END_TAGNAME,       EVENT_NONE },

  /* [19-21] handle end tag name */
  { ST_END_TAGNAME,   CLAZZ_LETTERS,      ST_END_TAGNAME,       EVENT_NONE },
  { ST_END_TAGNAME,   CLAZZ_RIGHT_ANGLE,  ST_START,             EVENT_END },
  /* below adds support for spaces at the end of an end tag (before
     closing bracket) */
  { ST_END_TAGNAME,   CLAZZ_SPACE,        ST_END_TAGNAME_END,   EVENT_END },

  /* [22] handle ending of end tag name */
  { ST_END_TAGNAME_END, CLAZZ_SPACE,      ST_END_TAGNAME_END,   EVENT_NONE },
  { ST_END_TAGNAME_END, CLAZZ_RIGHT_ANGLE,ST_START,             EVENT_NONE },

  /* [23-25] handle text */
  { ST_TEXT,          CLAZZ_SPACE,        ST_SPACE,             EVENT_NONE },
  { ST_TEXT,          CLAZZ_LEFT_ANGLE,   ST_START_TAG,         EVENT_TEXT },
  { ST_TEXT,          CLAZZ_ANY,          ST_TEXT,              EVENT_NONE },

  /* [26-30] handle attribute names */
  { ST_ATTR_NAME,     CLAZZ_LETTERS,      ST_ATTR_NAME,         EVENT_COPY },
  /* below add support for space before the equals sign, e.g, <tag
     attr ="2"> */
  { ST_ATTR_NAME,     CLAZZ_SPACE,        ST_ATTR_NAME_END,     EVENT_NAME },
  { ST_ATTR_NAME,     CLAZZ_EQUALS,       ST_ATTR_VAL,          EVENT_NAME },

  /* [31-33] attribute name end */
  { ST_ATTR_NAME_END, CLAZZ_SPACE,        ST_ATTR_NAME_END,     EVENT_NONE },
  { ST_ATTR_NAME_END, CLAZZ_LETTERS,      ST_ATTR_NAME,         EVENT_COPY },
  { ST_ATTR_NAME_END, CLAZZ_EQUALS,       ST_ATTR_VAL,          EVENT_NONE },

  /* [34-35] handle attribute values, initial quote and spaces */
  { ST_ATTR_VAL,      CLAZZ_QUOTE,        ST_ATTR_VAL2,         EVENT_NONE },
  /* below handles initial spaces before quoted attribute value */
  { ST_ATTR_VAL,      CLAZZ_SPACE,        ST_ATTR_VAL,          EVENT_NONE },

  /* [36-37] handle actual attribute values */
  { ST_ATTR_VAL2,     CLAZZ_QUOTE,        ST_START_TAGNAME_END, EVENT_VAL  },
  { ST_ATTR_VAL2,     CLAZZ_LETTERS,      ST_ATTR_VAL2,         EVENT_COPY },
  { ST_ATTR_VAL2,     CLAZZ_SLASH,        ST_ATTR_VAL2,         EVENT_NONE },


  /* End of table marker */
  { ST_ERROR,         CLAZZ_NONE,         ST_ERROR,             EVENT_NONE } 
};

struct tinyxml_t {
  int (*callback)(int event, 
                  const char* txt, int len,
                  void* user);
  void* user;
  char* tmp;
  int tmpix,            /* position in tmp buffer */
    tmpsz,              /* tmp buffer size */
    copy,               /* copy text into tmp buffer */
    halt,               /* halt parsing of document */
    state, event;       /* current state & event */
};

tinyxml_t* tinyxml_new(int bufsz,
                       int (*callback)(int event, 
                                       const char* txt, int len,
                                       void* user), 
                       void* user) {
  tinyxml_t* tinyxml = (tinyxml_t*) calloc(1, sizeof(tinyxml_t));
  tinyxml->callback = callback;
  tinyxml->user = user;
  tinyxml->tmpsz = bufsz;
  tinyxml->tmp = (char*) malloc(tinyxml->tmpsz);
  return tinyxml;
}

void tinyxml_free(tinyxml_t* tinyxml) {
  if(tinyxml) {
    free(tinyxml->tmp);
    free(tinyxml);
  }
}

const char* tinyxml_event_str(int event)
{
  switch(event) {
  case EVENT_START: return "start tag"; break;
  case EVENT_END: return "end tag"; break;
  case EVENT_TEXT: return "text"; break;
  case EVENT_NAME: return "attr name"; break;
  case EVENT_VAL: return "attr val"; break;
  case EVENT_END_DOC: return "end document"; break;
  default:
    break;
  }
  return "err";
}

int tinyxml_feed(tinyxml_t* tinyxml, const char* buf, int len)
{
  int i, j, c, type, match;
  for(i=0; i<len; i++) {
    if(tinyxml->halt) { break; }
    c = buf[i] & 0xff;
    type = char_type[c];
    for(j=0, match = 0; TINYXML_STATES[j].state != ST_ERROR; j++) {
      if(TINYXML_STATES[j].state != tinyxml->state) { continue; }
      switch(TINYXML_STATES[j].clazz) {
      case CLAZZ_LETTERS:
        match = (type == 1 || type == 2); /* see comment above for
                                             char_type */
        break;
      case CLAZZ_LEFT_ANGLE:  match = (c == '<');  break;
      case CLAZZ_SLASH:       match = (c == '/');  break;
      case CLAZZ_RIGHT_ANGLE: match = (c == '>');  break;
      case CLAZZ_EQUALS:      match = (c == '=');  break;
      case CLAZZ_QUOTE:       match = (c == '"');  break;
      case CLAZZ_SPACE:       match = (type == 3); break;
      case CLAZZ_ANY:         match = 1;           break;
      default:
        break;
      }
      if(match) { /* we matched a character class */
        if(TINYXML_STATES[j].event == EVENT_COPY) {
          tinyxml->copy = 1;
        } else if(TINYXML_STATES[j].event != EVENT_NONE) {
          if(tinyxml->copy) {
            /* basically we are guaranteed never to have an event of
               type EVENT_COPY or EVENT_NONE here. */
            tinyxml->event = TINYXML_STATES[j].event;
            tinyxml->tmp[tinyxml->tmpix] = 0; /* make a string */
            if(!tinyxml->callback(TINYXML_STATES[j].event, 
                                  tinyxml->tmp, tinyxml->tmpix , 
                                  tinyxml->user)) {
              tinyxml->halt = 1; /* stop parsing from here out */
            }
            tinyxml->tmpix = 0;
            tinyxml->copy = 0;
          }
        }
        if(tinyxml->copy) {
          /* check to see if we have room; one less for trailing
             nul */
          if(tinyxml->tmpix < tinyxml->tmpsz-1) {
            tinyxml->tmp[tinyxml->tmpix] = buf[i];
            tinyxml->tmpix++;
          }
        }
        tinyxml->state = TINYXML_STATES[j].next_state; /* change state */
        break; /* break out of loop though state search */
      }
    }    
  }  
  return !tinyxml->halt;
}

