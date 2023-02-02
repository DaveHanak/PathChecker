// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the FILEMETADATA_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// FILEMETADATA_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef FILEMETADATA_EXPORTS
#define FILEMETADATA_API __declspec(dllexport)
#else
#define FILEMETADATA_API __declspec(dllimport)
#endif

// This class is exported from the dll

extern "C"
{
    struct FILEMETADATA_API FileMetadata {

        unsigned int depth;
        wchar_t name[260];
        wchar_t type[23];
        wchar_t size[10];
        wchar_t lastWriteTime[20];
        wchar_t permissions[10];

        FileMetadata();
        FileMetadata(const std::filesystem::path file_path, const unsigned int directory_depth);

        ~FileMetadata() {}
    };

    typedef intptr_t FileMetadatasHandle;

    FILEMETADATA_API bool GetFileMetadatas(FileMetadatasHandle* metasHandle, FileMetadata** metas, const wchar_t* path, int* metasCount);
    FILEMETADATA_API bool ReleaseFileMetadatas(FileMetadatasHandle metasHandle);
}