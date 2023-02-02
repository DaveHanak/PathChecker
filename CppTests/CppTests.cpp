#include "pch.h"
#include "CppUnitTest.h"
#include "../FileMetadata/FileMetadata.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CppTests
{
	FileMetadatasHandle metasHandle;
	std::filesystem::path path = L"C:\\";
	std::wstring testingFolderName = L"CFileMetadata Testing Folder";
	std::wstring testingFileName = L"CFileMetadata Testing File.txt";
	std::string text = "lorem ipsum";
	std::wstring formattedLastWriteTime;

	TEST_CLASS(CppTests)
	{
	public:
		TEST_CLASS_INITIALIZE(CreateTestPaths)
		{
			path /= testingFolderName;
			path /= testingFileName;
			std::filesystem::create_directories(path.parent_path());

			std::ofstream ofs(path);
			ofs << text;
			ofs.close();
			formattedLastWriteTime = std::format(L"{}", std::filesystem::last_write_time(path)).substr(0, 19);

			std::filesystem::create_directories(path.parent_path() / L"one");
			std::filesystem::create_directories(path.parent_path() / L"one" / L"two");
			std::filesystem::create_directories(path.parent_path() / L"one" / L"two" / L"three");
			std::ofstream ofsOther(path.parent_path() / L"one" / L"two" / L"three" / L"four.txt");
			ofsOther << text;
			ofsOther.close();
		}
		TEST_METHOD(TestFileMetaName)
		{
			FileMetadata meta(path, 0);
			Assert::AreEqual(testingFileName.c_str(), meta.name);
		}
		TEST_METHOD(TestFileMetaType)
		{
			FileMetadata meta(path, 0);
			Assert::AreEqual(L"File", meta.type);
		}
		TEST_METHOD(TestFileMetaSize)
		{
			FileMetadata meta(path, 0);
			std::wstring expected_size = std::to_wstring(text.size()) + L" B";
			Assert::AreEqual(expected_size.c_str(), meta.size);
		}
		TEST_METHOD(TestFileMetaLastWriteTime)
		{
			FileMetadata meta(path, 0);
			Assert::AreEqual(formattedLastWriteTime.c_str(), meta.lastWriteTime);
		}
		TEST_METHOD(TestFileMetaPermissions)
		{
			FileMetadata meta(path, 0);
			const wchar_t* perms = L"rwxrwxrwx";
			Assert::AreEqual(perms, meta.permissions);
		}
		TEST_METHOD(TestFileMetasCount)
		{
			FileMetadata* metas;
			int metasCount = 0;
			GetFileMetadatas(&metasHandle, &metas, path.parent_path().wstring().c_str(), &metasCount);
			Assert::AreEqual(5, metasCount);
		}
		TEST_METHOD(TestFileMetasDepth)
		{
			FileMetadata* metas;
			int metasCount = 0;
			GetFileMetadatas(&metasHandle, &metas, path.parent_path().wstring().c_str(), &metasCount);
			Assert::AreEqual(metas[4].depth, 3u);
		}
		TEST_METHOD(TestFileMetaNonExistent)
		{
			try
			{
				FileMetadata* metas;
				int metasCount = 0;
				GetFileMetadatas(&metasHandle, &metas, L"nowhere", &metasCount);
				Assert::Fail(L"An exception should be thrown.");
			}
			catch (std::filesystem::filesystem_error) {}
		}
		TEST_METHOD_CLEANUP(ReleaseFileMetadatasHandle)
		{
			ReleaseFileMetadatas(metasHandle);
		}
		TEST_CLASS_CLEANUP(CleanupTestPaths)
		{
			std::filesystem::remove_all(path.parent_path());
		}
	};
}
