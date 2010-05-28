#ifndef _BASE64_HPP_
#define _BASE64_HPP_

namespace Base64 {
template <class T> class DataReference {
    T first;
    size_t second;
public:
    DataReference(T data, size_t size) {
        first=data;
        second=size;
    }
    static DataReference null() {
        return DataReference(NULL,0);
    }
    explicit DataReference(const std::string&s) {
        first=s.data();
        second=s.length();
    }
    template <class U> explicit DataReference(const std::vector<U> &v) {
        if (v.empty()) {
            first=NULL;
            second=0;
        }else {
            first=(T)&v[0];
            second=v.size();
        }
    }
    T begin() const {
        return first;
    }
    T end() const {
        // FIXME: If second really is measured in bytes, why do we use void* and not char*?
        return (void*)(((char*)first)+second);
    }
    T data() const {
        return first;
    }
    size_t size() const {
        return second;
    }
    inline size_t length() const {
        return size();
    }
};

typedef DataReference<const void*> MemoryReference;

BRAIN_CORE_EXPORT void toBase64(std::vector<unsigned char>&retval,
              const MemoryReference&firstInput, 
              const MemoryReference&secondInput);
BRAIN_CORE_EXPORT bool fromBase64(std::vector<unsigned char>&retval,
                const MemoryReference&a);
}

#endif
