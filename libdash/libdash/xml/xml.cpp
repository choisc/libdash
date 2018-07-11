#include "xml.h"
//#include "utf8.h"

#define CDATA_NODE 1
#define WHITESPACE_NODE 1
#define COMMENT_NODE 1
#define PI_NODE 1

#define xmlChar char
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <new>
    /*#ifdef _DEBUG
    #define Malloc(a) _malloc_base(a);
    #define Free(a) _free_base(a);
    #else*/
#define Malloc(a) malloc(a);
#define Free(a) free(a);
    /*#endif*/

    static xmlNode* xmlAddChild(xmlNode* parent, xmlNode* cur);

    static bool iswhite(const char c)
    {
        return ((c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'));
    }
    
    static const char* trimleft(const char* s)
    {
        for (;;)
        {
            if (iswhite(*s))
            {
                s++;
                continue;
            }
            break;
        }
        return s;
    }

    static char* trimright(char* s)
    {
        char* e = s;
        for (;;)
        {
            if (*s == 0) break;
            if (!iswhite(*s))
            {
                e = s + 1;
            }
            s++;
        }
        if (*e) *e = 0;
        return e;
    }

    static char* trimleft(char* s)
    {
        return (char*)trimleft((const char*)s);
    }

    static const char* find_first_of(const char* te, const char* t)
    {
        for (;;)
        {
            if (*te == 0) break;
            for (const char* f = t; *f != 0; f++)
            {
                if (*f == *te) return te;
            }
            te++;
        }
        return te;
    }

    static char* find_first_of(char* te, const char* t)
    {
        return (char*)find_first_of((const char*)te, t);
    }

    static const char* find_next_whitespace(const char* te)
    {
        for (;;)
        {
            if (*te == 0) break;
            if (iswhite(*te))
                break;
            te++;
        }
        return te;
    }

    static char* find_next_whitespace(char* te)
    {
        return (char*)find_next_whitespace((const char*)te);
    }

    // XMLATTR--------------------------
    xmlAttr::xmlAttr()
    {
        parent = NULL; next = NULL;
        name = NULL;
        content = NULL;
    }
    xmlAttr::~xmlAttr()
    {
        delete next;
        next = NULL;
    }

    // XMLNODE --------------------------
    xmlNode::xmlNode()
    {
        type = XML_ELEMENT_NODE;
        parent = NULL;
        attributes = NULL;
        name = NULL;
        content = NULL;
        isWhitespace = false;
        isEmpty = false;
        hasText = false;
        children = NULL;
        next = NULL;
        doc = NULL;
        lastchild = NULL;
    }

    xmlNode::~xmlNode()
    {
#if CUSTOM_ALLOC==1
#else
        if (isWhitespace)
        {
            Free((void*)content);
        }
        delete attributes;
        xmlNode* tmp;
        for (;;)
        {
            tmp = children;
            if (tmp == NULL) break;
            children = tmp->next;
            delete tmp;
        }
#endif
    }
    //NOTE: set_name is a bit of a misnomer, since it also parses the attributes from 'aName' which is actually the content of the tag..
    bool xmlNode::set_name(char* aName)
    {
        char quote[2] = { 0,0 };
        char* te = aName;
        name = te;
        te = find_next_whitespace(te);//name ends at whitespace.
        if (*te == 0) return true; //no white, so no attributes.
        *te = 0;//null terminate at first whitespace.
        te++;
        //tokenize..
        xmlAttr* previousAttr = NULL;
        xmlAttr** res = &attributes;
        for (;;)
        {
            te = trimleft(te);
            if (*te == 0) break;
            char* p = find_first_of(te, "=");
            if (*p == 0)
            {
                //corrupted document
                return false;
            }
            *p = 0;
            char *attr = te;

            //find end of attribute name and trim white space from right.
            char* trm = p - 1;
            //note: loop goes backwards, and it's impossible for it to fail..
            //basic sanity check would be 'if (trm==attr) then fail' but logic dictates that the loop MUST ALWAYS terminate before that, so we wont bother doing that.
            for (;;)
            {
                if (!iswhite(*trm))
                {
                    break;
                }
                *trm = 0;
                trm--;
            }

            char* value = p + 1; //skip '=' which was already changed to null terminator
            value = trimleft(value);  //trim whitespace.
            if ((*value != '\'') && (*value != '"'))
            {
                //corrupted document
                return false;
            }
            
            quote[0] = *value;     //store the starting char (single or double quote!)
            value += 1; //Skip the starting quote

            //parse content from tmp..
            //tmp[0] MUST BE " or ' (and must match the one it started with...)
            p = find_first_of(value, quote);
            if (*p == 0)
            {
                //corrupted document
                return false;
            }
            *p = 0;//terminate string at ending quote.
            
            xmlAttr* newAttr = doc->allocAttr();
            *res = newAttr;
            res = &newAttr->next;
            newAttr->parent = this;
            newAttr->next = NULL;
            newAttr->name = attr;
            newAttr->content = value;
            previousAttr = newAttr;
            te = p + 1;//continue parsing after the ending quote/terminator.
        }
        return true;
    }

    // XMLDOC ----------------------------------
    xmlDoc::xmlDoc()
    {
        doc = this;
        type = XML_DOCUMENT_NODE;
        root = NULL;
        children = NULL;
        data = NULL;
#if CUSTOM_ALLOC==1
        mNodeBlocks = 0;
        mAttrBlocks = 0;
        mWhitespaceBlocks = 0;
        mCurNodeBlock = NULL;
        mCurAttrBlock = NULL;
        mCurwhitespaceBlock = NULL;
        memset(mNodeBlock, 0, sizeof(mNodeBlock));
        memset(mAttrBlock, 0, sizeof(mAttrBlock));
        memset(mWhitespaceBlock, 0, sizeof(mWhitespaceBlock));
#endif
    }

    xmlDoc::~xmlDoc()
    {
        //delete root;
        //delete[] data;
        Free(data);
#if CUSTOM_ALLOC==1
        for (int i = 0; i < mNodeBlocks; i++)
        {
            Free(mNodeBlock[i]);
        }
        for (int i = 0; i < mAttrBlocks; i++)
        {
            Free(mAttrBlock[i]);
        }
        for (int i = 0; i < mWhitespaceBlocks; i++)
        {
            Free(mWhitespaceBlock[i]);
        }
#endif
    }

    char* xmlDoc::allocWhitespace(int len)
    {
        char* ret;
#if CUSTOM_ALLOC==1
        if ((mCurwhitespaceBlock == NULL) ||
            (mCurwhitespaceBlock->mFreeSpace < len))
        {
            int todo = 256;
            if (len > todo) todo = len;
            mWhitespaceBlock[mWhitespaceBlocks] = (whitespaceBlock*)Malloc(sizeof(whitespaceBlock) + todo);// new nodeBlock;

            mCurwhitespaceBlock = mWhitespaceBlock[mWhitespaceBlocks];
            mCurwhitespaceBlock->mAllocated = todo;
            mCurwhitespaceBlock->mFreeSpace = todo;
            mWhitespaceBlocks++;
        }
        ret = ((char*)mCurwhitespaceBlock->mWhitespace) + (mCurwhitespaceBlock->mAllocated - mCurwhitespaceBlock->mFreeSpace);
        mCurwhitespaceBlock->mFreeSpace -= len;
#else
        ret = (char*)Malloc(len);
#endif
        return ret;
    }

    xmlNode* xmlDoc::allocNode()
    {
        xmlNode* ret;
#if CUSTOM_ALLOC==1
        if ((mCurNodeBlock == NULL) ||
            (mCurNodeBlock->mNodes >= sizeof(mCurNodeBlock->mNode) / sizeof(xmlNode)))
        {
            mNodeBlock[mNodeBlocks] = (nodeBlock*)Malloc(sizeof(nodeBlock));// new nodeBlock;
            mCurNodeBlock = mNodeBlock[mNodeBlocks];
            //memset(mCurNodeBlock, 0, sizeof(nodeBlock));
            mCurNodeBlock->mNodes = 0;
            mNodeBlocks++;
        }
        void * addr = &mCurNodeBlock->mNode[mCurNodeBlock->mNodes * sizeof(xmlNode)];
        ret = new(addr) xmlNode();
        mCurNodeBlock->mNodes++;
#else
        ret = new xmlNode();
#endif
        ret->doc = this;
        return ret;
    }

    xmlAttr* xmlDoc::allocAttr()
    {
        xmlAttr* ret;
#if CUSTOM_ALLOC==1
        if ((mCurAttrBlock == NULL) ||
            (mCurAttrBlock->mAttributes >= sizeof(mCurAttrBlock->mAttribute) / sizeof(xmlAttr)))
        {
            mAttrBlock[mAttrBlocks] = (attrBlock*)Malloc(sizeof(attrBlock));// new attrBlock;
            mCurAttrBlock = mAttrBlock[mAttrBlocks];
            //memset(mCurAttrBlock, 0, sizeof(attrBlock));
            mCurAttrBlock->mAttributes = 0;
            mAttrBlocks++;
        }
        void* addr = &mCurAttrBlock->mAttribute[mCurAttrBlock->mAttributes * sizeof(xmlAttr)];
        ret = new(addr) xmlAttr();
        mCurAttrBlock->mAttributes++;
#else
        ret = new xmlAttr();
#endif
        ret->doc = this;
        return ret;
    }

    // MOCKXML  --------------------------
#if 0
    void MockXml::deescape(std::string& data)
    {
        //decode..
        int pos = 0;
        for (;;)
        {
            //Technically &XXXXX; is an entity ref. and could be anything!
            //but since i dont even parse entity definitions..
            int np = data.find_first_of('&', pos);
            if (np == std::string::npos) break;
            const char *t = data.c_str() + np;
            char res = 0;
            int len = 0;
            if ((t[1] == 'l') && (t[2] == 't') && (t[3] == ';')) { len = 3; res = '<'; }
            else if ((t[1] == 'g') && (t[2] == 't') && (t[3] == ';')) { len = 3; res = '>'; }
            else if ((t[1] == 'a') && (t[2] == 'm') && (t[3] == 'p') && (t[4] == ';')) { len = 4; res = '&'; }
            else if ((t[1] == 'a') && (t[2] == 'p') && (t[3] == 'o') && (t[4] == 's') && (t[5] == ';')) { len = 5; res = '\''; }
            else if ((t[1] == 'q') && (t[2] == 'u') && (t[3] == 'o') && (t[4] == 't') && (t[5] == ';')) { len = 5; res = '\"'; }
            else if ((t[1] == 'n') && (t[2] == 'b') && (t[3] == 's') && (t[4] == 'p') && (t[5] == ';'))
            {
                /*nbsp*/
                len = 5;
                data.erase(np, len - 1);
                data[np] = 0xC2; np++;
                data[np] = 0xA0; np++;
                pos = np;
                res = 0;
            }
            else if (t[1] == '#')
            {
                //hexcodes! (or not?)
                char tmp[256];
                int i;
                for (i = 0;; i++)
                {
                    if (t[i + 2] == ';') break;
                    tmp[i] = t[i + 2];
                }
                tmp[i] = 0;
                int chr;
                sscanf(tmp, "%d", &chr);
                //TODO: utf 8 convert it...
                //store it..            
                int cnt;
                UTF8::encode((unsigned char*)tmp, 255, &cnt, chr);
                tmp[cnt] = 0;
                data.erase(np, len + i + 3);
                data.insert(np, tmp);
                pos = np;
            }
            if (res)
            {
                data.erase(np, len);
                data[np] = res;
                pos = np;
            }
            pos++;
        }
    }
#endif

    typedef struct
    {
        int len;
        const char* start;
    } PendingWhite;

    xmlNode* xmlAddChild(xmlNode* parent, xmlNode* cur)
    {
        if (parent == NULL)
        {
            //NULL means add to document node.
            parent = cur->doc;
        }

        if (cur->type == XML_TEXT_NODE)//wonder if i should tag cdatas here too... hmm. they seem to be correct (this affects the NO_BLANKS parsing only)
        {
            parent->hasText = true;
        }

        cur->parent = parent;
        if (parent->lastchild)
        {
            parent->lastchild->next = cur;
            parent->lastchild = cur;
        }
        else
        {
            parent->children = cur;
            parent->lastchild = cur;
        }
        return cur;
    }

    static int parse_decl(const char* src, const char* end, xmlDoc*& doc)
    {
        //verify that we start with '<?xml ' !!!
        //<?xml version="1.0" encoding="UTF-8"?>
        if ((src[0] != '<') &&
            (src[1] != '?') &&
            (src[2] != 'x') &&
            (src[3] != 'm') &&
            (src[4] != 'l') &&
            iswhite(src[5]))
        {
            //not an xml document...
            return 0;
        }
        //TODO: should parse the declaration...
        //ie. version and encoding..

        int len = 0;
        const char* tag_end;
        const char* tag_start;
        src++;
        tag_start = src;
        for (;;)
        {
            if (src>=end)
            {
                //corrupted document
                return 0;
            }
            if (*src == 0)
            {
                //corrupted document
                return 0;
            }
            if (*src == '>') break;
            src++;
            len++;
        }
        len--;
        tag_end = src - 1;
        if (*tag_end != '?')
        {
            //corrupted document.
            return 0;
        }
        //seems okay..
        doc = new xmlDoc();
        //Not correct.. need to parse the attributes and store differently.. (not as attribs...)               
        /*char* tmp = doc->allocWhitespace(len);
        memcpy(tmp, tag_start, len);
        tmp[len] = 0;
        doc->set_name(tmp);
        doc->name = NULL;*/
        //and no hardcoding.. :)
        doc->version = "1.0";
        doc->encoding = "utf-8";

        return len + 3;
    }
    void do_pending_white(xmlDoc* doc,xmlNode*cur,PendingWhite& pendingWhite,int open,bool no_blanks)
    {
        int len = pendingWhite.len;
        if (len == 0) return;
        pendingWhite.len = 0;
        if (doc->root == NULL) return;
        if (no_blanks)
        {
            if (open == 1)
            {
                //opening new tag..
                //so skip the pending white?
                if (cur->hasText == false)
                {
                    return;
                }
            }
            if (open == -1)
            {
                //cant have whites?
                return;
            }
            if (open == 0)
            {
                if ((cur->children)&&(cur->hasText==false))
                {
                    return;
                }
            }
        }
        xmlNode* tmp;
        tmp = doc->allocNode();
        tmp->name = "text";
        tmp->type = XML_TEXT_NODE;
        tmp->isWhitespace = true;
        //need to copy the whitespace characters, because we cant add null termination at the end..
        tmp->content = doc->allocWhitespace(len + 1);
        memcpy((char*)tmp->content, pendingWhite.start, len);
        ((char*)tmp->content)[len] = 0;
        xmlAddChild(cur, tmp);
    }
    static xmlDoc* parse_xml(const void* src_data, int src_len,bool no_blanks,bool copy)
    {
        char* src;
        char* end;
        src = end = NULL;
        //check for bom.. 
        char* sr = (char*)src_data;
        //just skip the UTF-8 BOM..
        //any others will just fail the XML declaration parsing..
        if (((unsigned char)sr[0] == 0xEF) && ((unsigned char)sr[1] == 0xBB) && ((unsigned char)sr[2] == 0xBF))
        {
            sr += 3;
            src_len-=3;
        }
        if (src_len<7) 
        {
            //No way this is valid.
            //since the "<?xml?>" does not fit.
            return NULL;
        }
        xmlDoc* doc;
        int decl_len = parse_decl(sr, sr+src_len,doc);
        if (decl_len == 0)
        {
            //not an xml
            return NULL;
        }
        sr += decl_len;
        src_len -= decl_len;
        if (copy)
        {
            doc->data = (char*)Malloc(src_len);
            src = doc->data;
        }
        else
        {
            doc->data = (char*)src_data;
            src = sr;
        }
        //normalize and check length.
        end = src;
        const char* src_end = sr + src_len;
        for (;;)
        {
            if (sr == src_end)
            {
                break;
            }
            if (*sr == 0)
            {
                //null termination reached before end of data?
                delete doc;
                return NULL;
            }
            if (*sr == 0xD)//'\d'
            {
                *end++ = 0xA;//'\a'
                sr++;
                if (*sr == 0) continue;
                if (*sr == 0xA)//'\a'
                {
                    sr++;
                }
                continue;
            }
            *end++ = *sr++;
        }
        src_len = end - src;
        PendingWhite pendingWhite = { 0 };
        xmlNode* cur = NULL;
        bool haveRoot = false;
        /*
        TODO: ]]> is not allowed in text. (or basically anywhere else than end of cdata.)
              no validation of characters in attribute/tag names.
              no charref/entity replacement.
        */
        for (;;)
        {
            if (src >= end) break;
            haveRoot = (doc->root != NULL);
            if (src[0] == '<')
            {
                *src = 0;//terminate the previous... (and remove the '<')
                src++;
                if (src[0] == '!')  //CData or comment.
                {
                    if ((src[1] == '[') && (src[2] == 'C') && (src[3] == 'D') && (src[4] == 'A') && (src[5] == 'T') && (src[6] == 'A') && (src[7] == '['))
                    {
                        do_pending_white(doc, cur, pendingWhite, 0, no_blanks);
                        //CDATA
                        //find the end&count len
                        char* ssrc = src + 8;
                        int data_len = 0;
                        src = ssrc;
                        for (;;)
                        {
                            if (src>=end)
                            {
                                //end of data found before end of cdata.
                                delete doc;
                                return NULL;
                            }
                            if ((src[0] == ']') && (src[1] == ']') && (src[2] == '>'))
                            {
                                //end of cdata..
                                *src = 0;
                                src += 3;
                                break;
                            }
                            data_len++;
                            src++;
                        }
#if CDATA_NODE
                        xmlNode* tmp = cur->doc->allocNode();
                        tmp->type = XML_CDATA_SECTION_NODE;
                        tmp->isEmpty = true;
                        tmp->content = ssrc;
                        xmlAddChild(cur, tmp);
#endif                        
                    }
                    else if ((src[1] == '-') && (src[2] == '-'))
                    {
                        do_pending_white(doc, cur, pendingWhite, 0, no_blanks);
                        char* tag_start = src + 3;
                        //comment..
                        //scan to the end of comment.. 
                        src = tag_start;
                        for (;;)
                        {
                            if (src>=end)
                            {
                                //end of data found before end of comment.
                                delete doc;
                                return NULL;
                            }
                            if ((src[0] == '-') && (src[1] == '-'))
                            {
                                if ((src[2] == '>'))
                                {
                                    //end of comment.
                                    //just flush these out
                                    *src = 0;
                                    src += 3;
                                    break;
                                }
                                //must be a corrupted document then
                                //double hyphen is not allowed in comments
                                delete doc;
                                return NULL;
                            }
                            src++;
                        }
#if COMMENT_NODE
                        xmlNode* tmp;
                        tmp = doc->allocNode();
                        tmp->type = XML_COMMENT_NODE;
                        tmp->isEmpty = true;
                        tmp->name = "comment";
                        tmp->content = tag_start;
                        xmlAddChild(cur, tmp);
#endif
                    }
                    else
                    {
                        //UMM what?
                        //!ELEMENT , !DOCTYPE, !ATTLIST, !ENTITY... which i do not parse yet!
                        delete doc;
                        return NULL;
                    }
                }
                else
                {
                    //Is tag
                    char* tag_start;
                    char* tag_end;
                    tag_start = src;
                    if (iswhite(*tag_start))
                    {
                        //tag name must not contain whitespace..
                        //and it should start immediately after the <
                        //ie. '< ' sequences are invalid.
                        delete doc;
                        return NULL;
                    }
                    for (;;)
                    {
                        if (src>=end)
                        {
                            //end of data found before end of tag.
                            delete doc;
                            return NULL;
                        }
                        if (*src == '>') break;
                        src++;
                    }
                    tag_end = src;
                    *src = 0;//clip the tag.. in-situ
                    src++;
                    if (tag_start[0] == '/')
                    {
                        //closing tag.. (better match the current open tag)
                        //TODO: check that the tags match!
                        do_pending_white(doc, cur, pendingWhite, 0,no_blanks);
                        const char* partag = cur->name;
                        if (strcmp(partag, tag_start+1) != 0)
                        {
                            //Tags do not match. corrupted xml
                            delete doc;
                            return NULL;
                        }
                        if (cur == doc->root)
                        {
                            //we are stopping the parse here, and ignoring the rest of data (if any)!
                            //the document SHOULD be completely parsed now as the root node is closed..
                            //technically we should return an error if there is extra content (anything else than comments) in the input data.
                            cur = NULL;
                            break;
                        }
                        cur = cur->parent;//go back a node..
                    }
                    else if (tag_start[0] == '?')
                    {
                        //PROCESSING INSTRUCTION (not the XML declaration)
                        do_pending_white(doc, cur, pendingWhite, 0, no_blanks);
                        if (*(tag_end - 1) != '?')
                        {
                            //invalid document. PI's must start and end with ?
                            //SGML does not have the ending requirement.(but we dont support that now)
                            delete doc;
                            return NULL;
                        }
                        if (iswhite(*(tag_start + 1)))
                        {
                            //target missing. (cant start with whitespace)
                            delete doc;
                            return NULL;
                        }
#if PI_NODE
                        xmlNode* tmp;
                        tmp = doc->allocNode();
                        tmp->type = XML_PI_NODE;
                        tmp->isEmpty = true;
                        *(tag_end - 1) = 0;//cut out the '?'
                        tmp->name = tag_start + 1;
                        xmlAddChild(cur, tmp);
#endif
                    }
                    else if (*(tag_end - 1 ) == '/')
                    {
                        do_pending_white(doc, cur, pendingWhite, -1, no_blanks);
                        //empty tag.
                        xmlNode* tmp;
                        tmp = doc->allocNode();
                        tmp->type = XML_ELEMENT_NODE;
                        tmp->isEmpty = true;
                        *(tag_end - 1) = 0;//cut out the '/'
                        tmp->set_name(tag_start);
                        xmlAddChild(cur, tmp);
                        if (doc->root == NULL) doc->root = tmp;
                    }
                    else
                    {
                        //new tag.
                        do_pending_white(doc, cur, pendingWhite, 1, no_blanks);
                        xmlNode* tmp;
                        tmp = doc->allocNode();
                        tmp->type = XML_ELEMENT_NODE;
                        tmp->set_name(tag_start);
                        xmlAddChild(cur, tmp);
                        cur = tmp;
                        if (doc->root == NULL) doc->root = tmp;
                    }
                }
            }
            else
            {
                //must be raw "character data" then..
                //ie. everything up until '<' is data..
                char* data_start = src;
                bool whiteonly = true;
                for (;;)
                {
                    if (src>=end)
                    {
                        //end of file while in character data.
                        delete doc;
                        return NULL;
                    }
                    if (*src == '<') break;
                    if (!iswhite(*src))
                    {
                        whiteonly = false;
                        break;
                    }
                    src++;
                }
                if (whiteonly)//ie. contains only whitespace.
                {
                    //store as pending, add the node depending on noblanks rules.
#if WHITESPACE_NODE
                    int len = src - data_start;
                    if (len)
                    {
                        pendingWhite.len = len;
                        pendingWhite.start = data_start;
                    }
#endif
                } 
                else
                {
                    pendingWhite.len = 0;
                    //find end..
                    for (;;)
                    {
                        if (src>=end)
                        {
                            //end of data while in character data.
                            delete doc;
                            return NULL;
                        }
                        if (*src == '<') break;
                        src++;
                    }
                    xmlNode* tmp;
                    tmp = doc->allocNode();
                    tmp->type = XML_TEXT_NODE;
                    tmp->name = "text";
                    tmp->isEmpty = true;
                    tmp->content = data_start;
                    xmlAddChild(cur, tmp);
                }
            }
        }
        src = NULL;
        end = NULL;
        if (cur)
        {
            //Partial document, since open tags.
            delete doc;
            doc = NULL;
        }
        return doc;
    }
    static xmlDoc* parse_xml(const char* a, bool no_blanks)
    {
        FILE* f = fopen(a, "rb");
        fseek(f, 0, SEEK_END);
        int s = ftell(f);
        fseek(f, 0, SEEK_SET);
        char* xdata = new char[s];
        fread(xdata, 1, s, f);
        fclose(f);
        xmlDoc* ret = parse_xml(xdata, s, no_blanks,false/*dont copy data, instead take ownership*/);
        return ret;
    }

    class xmlTextReader
    {
    public:
        xmlTextReader()
        {
            mDocument = NULL;
            mCurrentNode = NULL;
            mCurrentAttr = NULL;
            mNodeStackDepth = 0;
            eof = false;
        }
        ~xmlTextReader()
        {
            delete mDocument;
        }
        int mType;
        xmlDoc* mDocument;
        xmlNode* mCurrentNode;
        xmlAttr* mCurrentAttr;
        int mNodeStackDepth;// Debug purposes.
        bool eof;
    };

    xmlTextReaderPtr xmlReaderForMemory(const void* mData, unsigned int mSize, const char* url, void*, unsigned int option)
    {
        xmlTextReaderPtr ret = new xmlTextReader;
        ret->mDocument = parse_xml(mData, mSize, option&XML_PARSE_NOBLANKS,true/*copy data*/);
        return ret;

    }
    xmlTextReaderPtr xmlReaderForFile(const char* url, void*, unsigned int option)
    {
        xmlTextReaderPtr ret = new xmlTextReader;
        ret->mDocument = parse_xml(url, option&XML_PARSE_NOBLANKS);
        return ret;
    }

    static int type(xmlNode* n)
    {
        int Type = XML_READER_TYPE_ELEMENT;//node opens.
        if (n->type == XML_COMMENT_NODE)
            Type = XML_READER_TYPE_COMMENT;
        if (n->type == XML_PI_NODE)
            Type = XML_READER_TYPE_PROCESSING_INSTRUCTION;
        if (n->type == XML_TEXT_NODE)
        {
            if (n->isWhitespace)
                Type = XML_READER_TYPE_SIGNIFICANT_WHITESPACE;
            else
                Type = XML_READER_TYPE_TEXT;
        }
        if (n->type == XML_CDATA_SECTION_NODE)
            Type = XML_READER_TYPE_CDATA;
        return Type;
    }
    static void advance(xmlTextReaderPtr p)
    {
        //advance to sibling
        xmlNode* tmp = p->mCurrentNode;
        p->mCurrentNode = p->mCurrentNode->next;
        if (p->mCurrentNode)
        {
            //Open sibling.
            p->mType = type(p->mCurrentNode);
        }
        else
        {
            //no siblings.. close parent then
            //and move up the stack..
            if (p->mNodeStackDepth > 0)
            {
                p->mNodeStackDepth--;
                p->mCurrentNode = tmp->parent;
                p->mType = XML_READER_TYPE_END_ELEMENT;
            }
        }
    }
    int xmlTextReaderRead(xmlTextReaderPtr p)
    {
        p->mCurrentAttr = NULL;
        if (p->mCurrentNode == NULL)
        {
            if (p->eof)
            {
                return 0;
            }
            p->eof = true;
            p->mCurrentNode = p->mDocument->children;// ->root;
            p->mType = type(p->mCurrentNode);
            return 1;
        }
        if (p->mType == XML_READER_TYPE_ELEMENT)
        {
            // advance to children first
            if (p->mCurrentNode->children)
            {
                p->mCurrentNode = p->mCurrentNode->children;
                p->mNodeStackDepth++;
                p->mType = type(p->mCurrentNode);
            }
            else
            {
                // if no children, close the node
                p->mType = XML_READER_TYPE_END_ELEMENT;//node closed
            }
        }
        else
        {
            advance(p);
        }

        if (p->mCurrentNode)
        {
            if (p->mType == XML_READER_TYPE_END_ELEMENT)
            {
                //check if we actually WANT to return this..
                if (p->mCurrentNode->isEmpty)
                {
                    //nope.. so return the next one then..
                    return xmlTextReaderRead(p);
                }
            }
            return 1;
        }
        return 0;
    }

    int xmlTextReaderNodeType(xmlTextReaderPtr p)
    {
        /*      Start = 1,
        End = 15,
        Comment = 8,
        WhiteSpace = 14,
        Text = 3,
        */
        return p->mType;
    }
    const char* xmlTextReaderConstName(xmlTextReaderPtr p)
    {
        if (p->mCurrentAttr == NULL)
        {
            if (p->mType == XML_READER_TYPE_TEXT)
            {
                return "#text";
            }
            if (p->mType == XML_READER_TYPE_COMMENT)
            {
                return "#comment";
            }
            if (p->mType == XML_READER_TYPE_SIGNIFICANT_WHITESPACE)
            {
                return "#text";
            }
            if (p->mType == XML_READER_TYPE_CDATA)
            {
                return "#cdata-section";
            }
            return p->mCurrentNode->name;
        }
        else
        {
            return p->mCurrentAttr->name;
        }
    }
    const char* xmlTextReaderConstValue(xmlTextReaderPtr p)
    {
        //TODO: unescape/normalize here if needed..
        if (p->mCurrentAttr == NULL)
        {
            return p->mCurrentNode->content;
        }
        else
        {
            return p->mCurrentAttr->content;
        }
    }
    int xmlTextReaderIsEmptyElement(xmlTextReaderPtr p)
    {
        if (p->mCurrentNode->children) return 0;
        return 1;
    }
    const char* xmlTextReaderReadString(xmlTextReaderPtr p)
    {
        return p->mCurrentNode->content;
    }
    void xmlFree(const char*)
    {
        //ignore
    }
    int xmlTextReaderHasAttributes(xmlTextReaderPtr p)
    {
        if (p->mCurrentNode->attributes) return 1;
        return 0;
    }
    int xmlTextReaderMoveToFirstAttribute(xmlTextReaderPtr p)
    {
        p->mCurrentAttr = p->mCurrentNode->attributes;
        if (p->mCurrentAttr) return 1;
        return 0;
    }
    int xmlTextReaderMoveToNextAttribute(xmlTextReaderPtr p)
    {
        if (p->mCurrentAttr == NULL)
        {
            p->mCurrentAttr = p->mCurrentNode->attributes;
        }
        else
        {
            p->mCurrentAttr = p->mCurrentAttr->next;
        }
        if (p->mCurrentAttr) return 1;
        return 0;
    }

    void xmlFreeTextReader(xmlTextReaderPtr p)
    {
        delete p;
    }

    xmlDocPtr xmlReadFile(const char * filename, const char * encoding, int options)
    {
        return parse_xml(filename, options&XML_PARSE_NOBLANKS);
    }

    xmlDocPtr   xmlReadMemory(const char * buffer, int size, const char * URL, const char * encoding, int options)
    {
        return parse_xml(buffer, size, options&XML_PARSE_NOBLANKS,true/*copy input data*/);
    }

    xmlNodePtr  xmlDocGetRootElement(const xmlDoc * doc)
    {
        return doc->root;
    }

    void        xmlFreeDoc(xmlDocPtr doc)
    {
        delete doc;
    }
