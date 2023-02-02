// FileMetadata.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "FileMetadata.h"

FileMetadata::FileMetadata() : depth(260u)
{
    wcscpy_s(name, L"");
    wcscpy_s(type, L"");
    wcscpy_s(size, L"");
    wcscpy_s(lastWriteTime, L"");
    wcscpy_s(permissions, L"");
}

FileMetadata::FileMetadata(const std::filesystem::path file_path, const unsigned int directory_depth)
{
    // Depth
    depth = directory_depth;

    // Name
    wcscpy_s(name, file_path.filename().wstring().c_str());

    // Type
    switch (std::filesystem::status(file_path).type())
    {
    case std::filesystem::file_type::none:      wcscpy_s(type, L"None"); break;
    case std::filesystem::file_type::not_found: wcscpy_s(type, L"Not found"); break;
    case std::filesystem::file_type::regular:   wcscpy_s(type, L"File"); break;
    case std::filesystem::file_type::directory: wcscpy_s(type, L"Directory"); break;
    case std::filesystem::file_type::symlink:   wcscpy_s(type, L"Symlink"); break;
    case std::filesystem::file_type::block:     wcscpy_s(type, L"Block"); break;
    case std::filesystem::file_type::character: wcscpy_s(type, L"Character file"); break;
    case std::filesystem::file_type::fifo:      wcscpy_s(type, L"Pipe"); break;
    case std::filesystem::file_type::socket:    wcscpy_s(type, L"Socket"); break;
    case std::filesystem::file_type::unknown:   wcscpy_s(type, L"Unknown"); break;
    default:                                    wcscpy_s(type, L"Implementation-defined"); break;
    }

    // Size
    unsigned long long temp_size = std::filesystem::file_size(file_path);
    int exponent = 0;
    double mantissa = static_cast<double>(temp_size);
    for (; mantissa >= 1024.; mantissa /= 1024., ++exponent) {}
    mantissa = std::ceil(mantissa * 10.) / 10.;
    std::wstring s_size = std::to_wstring(mantissa);
    if (exponent == 0)
    {
        s_size = s_size.substr(0, s_size.find_last_of(L".")) + L" B";
    }
    else
    {
        s_size = s_size.substr(0, s_size.find_last_of(L".") + 2) + L" " + L"KMGTPE"[exponent - 1] + L"B";
    }

    wcscpy_s(size, s_size.c_str());

    // Last write time
    wcscpy_s(lastWriteTime, (std::format(L"{}", std::filesystem::last_write_time(file_path))).substr(0, 19).c_str());

    // Permissions
    std::filesystem::perms temp_perms = std::filesystem::status(file_path).permissions();
    std::wstring s_permissions;
    (temp_perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? s_permissions += L"r" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? s_permissions += L"w" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? s_permissions += L"x" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::group_read) != std::filesystem::perms::none ? s_permissions += L"r" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::group_write) != std::filesystem::perms::none ? s_permissions += L"w" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? s_permissions += L"x" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::others_read) != std::filesystem::perms::none ? s_permissions += L"r" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::others_write) != std::filesystem::perms::none ? s_permissions += L"w" : s_permissions += L"-";
    (temp_perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? s_permissions += L"x" : s_permissions += L"-";
    wcscpy_s(permissions, s_permissions.c_str());
}

bool GetFileMetadatas(FileMetadatasHandle* metasHandle, FileMetadata** metas, const wchar_t* path, int* metasCount)
{
    auto metasTemp = new std::vector<FileMetadata>();
    for (auto i = std::filesystem::recursive_directory_iterator(path); i != std::filesystem::recursive_directory_iterator(); ++i)
    {
        metasTemp->push_back(FileMetadata(i->path(), i.depth()));
    }
    *metasHandle = reinterpret_cast<FileMetadatasHandle>(metasTemp);
    *metas = metasTemp->data();
    *metasCount = metasTemp->size();
    return true;
}

bool ReleaseFileMetadatas(FileMetadatasHandle metasHandle)
{
    auto metas = reinterpret_cast<std::vector<FileMetadata>*>(metasHandle);
    delete metas;
    return true;
}