#ifndef _MOCKXML_
#define _MOCKXML_
#define CUSTOM_ALLOC 0
#define XML_PARSE_NOBLANKS 256
    //check  http://www.w3.org/TR/REC-DOM-Level-1/ 
    //We are not in complete sync here.
#define XML_ELEMENT_NODE 1
#define XML_TEXT_NODE 3
#define XML_CDATA_SECTION_NODE 4
#define XML_PI_NODE 7
#define XML_COMMENT_NODE 8
#define XML_DOCUMENT_NODE 9
    
    
#define XML_READER_TYPE_ELEMENT 1
#define XML_READER_TYPE_TEXT 3
#define XML_READER_TYPE_CDATA 4
#define XML_READER_TYPE_PROCESSING_INSTRUCTION 7
#define XML_READER_TYPE_COMMENT 8
#define XML_READER_TYPE_SIGNIFICANT_WHITESPACE 14
#define XML_READER_TYPE_END_ELEMENT 15

    class xmlDoc;
    class xmlNode;
    class xmlAttr
    {
    public:
        xmlAttr();
        ~xmlAttr();
        const char* name;
        const char* content;
        xmlNode* parent;
        xmlAttr* next;//pointer to next sibling...
        xmlDoc* doc;
    };

    class xmlNode
    {
    public:
        xmlNode();
        virtual ~xmlNode();
        bool set_name(char* aName);//aName is modified and tokenized. and should outlive this node.
        const char* name;
        const char* content;
        int type;
        bool isWhitespace;//text node containing only whitespace.
        bool isEmpty;// is of <tag/> type (ie. can NOT have children)
        bool hasText;//has a text child
        xmlAttr* attributes;
        xmlNode* parent;
        xmlNode* children;//pointer to first child
        xmlNode* next;//pointer to next sibling...
        xmlNode* lastchild;//pointer to last child
        xmlDoc* doc;
    };

    class xmlDoc:public xmlNode
    {
    public:
        xmlDoc();
        ~xmlDoc();
        xmlNode* root;//first actual node.
        const char* version;
        const char* encoding;

    //internal stuff.
        char* data;
        xmlNode* allocNode();
        xmlAttr* allocAttr();
        char* allocWhitespace(int len);
    protected:
#if CUSTOM_ALLOC==1
        int mNodeBlocks;
        class nodeBlock
        {
        public:
            int mNodes;
            unsigned char mNode[16384 * sizeof(xmlNode)];
        };
        nodeBlock* mNodeBlock[64];
        nodeBlock* mCurNodeBlock;
        //attr 
        int mAttrBlocks;
        class attrBlock
        {
        public:
            int mAttributes;
            unsigned char mAttribute[16384 * sizeof(xmlAttr)];
        };
        attrBlock* mAttrBlock[64];
        attrBlock* mCurAttrBlock;
        //whitespace
        int mWhitespaceBlocks;
        class whitespaceBlock
        {
        public:
            int mFreeSpace;
            int mAllocated;
            unsigned char mWhitespace[];
        };
        whitespaceBlock* mWhitespaceBlock[64];
        whitespaceBlock* mCurwhitespaceBlock;
#endif
    };

    //Libxml2 like apis.

    //tree api
    typedef xmlDoc* xmlDocPtr;
    typedef xmlNode* xmlNodePtr;
    xmlDocPtr   xmlReadFile(const char * filename, const char * encoding, int options);
    xmlDocPtr   xmlReadMemory(const char * buffer, int size, const char * URL, const char * encoding, int options);
    xmlNodePtr  xmlDocGetRootElement(const xmlDoc * doc);
    void        xmlFreeDoc(xmlDocPtr cur);

    //reader api
    class xmlTextReader;
    typedef xmlTextReader* xmlTextReaderPtr;
    xmlTextReaderPtr xmlReaderForMemory(const void* mData, unsigned int mSize, const char* url, void*, unsigned int);
    xmlTextReaderPtr xmlReaderForFile(const char* url, void*, unsigned int);

    int xmlTextReaderRead(xmlTextReaderPtr);

    int xmlTextReaderNodeType(xmlTextReaderPtr);
    const char* xmlTextReaderConstName(xmlTextReaderPtr);
    const char* xmlTextReaderConstValue(xmlTextReaderPtr);
    int xmlTextReaderIsEmptyElement(xmlTextReaderPtr);
    const char* xmlTextReaderReadString(xmlTextReaderPtr);
    void xmlFree(const char*);
    int xmlTextReaderHasAttributes(xmlTextReaderPtr);
    int xmlTextReaderMoveToFirstAttribute(xmlTextReaderPtr);
    int xmlTextReaderMoveToNextAttribute(xmlTextReaderPtr);

    void xmlFreeTextReader(xmlTextReaderPtr);
#endif
