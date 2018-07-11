#pragma once
#include <string>
typedef char char_t;
const size_t Npos = std::string::npos;
    class Uri
    {
    public:
        Uri();
        Uri(const Uri& aUri);
        Uri(const std::string& aString);
        Uri(const char_t* aString,size_t aLength=Npos);
        ~Uri();
  
        void resolve(const char_t* aString, size_t aLength = Npos);
        void resolve(const std::string& other);
        void resolve(const Uri& other);
        
        const std::string& toString() const;
        Uri& operator=(const Uri& other);
        bool operator==(const Uri& other) const;
        bool operator!=(const Uri& other) const;    
    protected:
        void parse(const char_t* aString, size_t aLength = Npos);
        class StringView
        {
        public:
            //Helper class, to reduce copying. does not own the string data.
            StringView();
            StringView(const char_t* const aData,size_t aLength );
            ~StringView();
            
            void rebase(const char_t* const orig_base, const char_t* const new_base);
            bool isEmpty() const;
            size_t getLength() const;
            const char_t* const getData() const;
            size_t FindFirst(char_t chr, size_t offset = Npos) const;
            size_t FindLast(char_t chr, size_t offset = Npos) const;
            StringView substring(size_t start, size_t length = Npos) const;
            void clear();
            const char_t operator[](size_t) const;
            bool operator==(const StringView&) const;
            bool operator!=(const StringView&) const;            
        private:
            size_t mLength;
            const char_t* mData;
        };    
        void merge(const Uri&base, const Uri&ref, std::string& res);
        void remove_dot_segments(const StringView& path, std::string& result);
        std::string mString;
        StringView mScheme;
        StringView mAuthority;
        StringView mPath;
        StringView mQuery;
        StringView mFragment;
    };
