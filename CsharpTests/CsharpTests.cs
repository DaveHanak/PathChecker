using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Runtime.InteropServices;
using static PathChecker.PathCheckerApp;

namespace CsharpTests
{
    [TestClass]
    public class CsharpTests
    {
        readonly string iexploreFolder = Environment.ExpandEnvironmentVariables("%ProgramW6432%") + "\\Internet Explorer";

        [TestMethod]
        public void TestFileMetasFoundName()
        {
            using (GetFileMetadatasWrapper(out IntPtr metas, iexploreFolder, out int metasCount))
            {
                int structSize = Marshal.SizeOf(typeof(FileMetadata));
                IntPtr metasTemp = metas;
                bool foundIexplore = false;
                for (int i = 0; i < metasCount; ++i)
                {
                    FileMetadata fm = (FileMetadata)Marshal.PtrToStructure(metasTemp, typeof(FileMetadata));
                    metasTemp += structSize;
                    if (fm.name == "iexplore.exe") foundIexplore = true;
                }
                Assert.IsTrue(foundIexplore);
            }
        }

        [TestMethod]
        public void TestFileMetasFoundType()
        {
            using (GetFileMetadatasWrapper(out IntPtr metas, iexploreFolder, out int metasCount))
            {
                int structSize = Marshal.SizeOf(typeof(FileMetadata));
                IntPtr metasTemp = metas;
                string fileType = "not found";
                for (int i = 0; i < metasCount; ++i)
                {
                    FileMetadata fm = (FileMetadata)Marshal.PtrToStructure(metasTemp, typeof(FileMetadata));
                    metasTemp += structSize;
                    if (fm.name == "images") fileType = fm.type;
                }
                Assert.AreEqual("Directory", fileType);
            }
        }

        [TestMethod]
        public void TestFileMetasFoundSize()
        {
            using (GetFileMetadatasWrapper(out IntPtr metas, iexploreFolder, out int metasCount))
            {
                int structSize = Marshal.SizeOf(typeof(FileMetadata));
                IntPtr metasTemp = metas;
                string fileSize = "not found";
                for (int i = 0; i < metasCount; ++i)
                {
                    FileMetadata fm = (FileMetadata)Marshal.PtrToStructure(metasTemp, typeof(FileMetadata));
                    metasTemp += structSize;
                    if (fm.name == "iexplore.exe") fileSize = fm.size;
                }
                Assert.AreEqual("815.0 KB", fileSize);
            }
        }

        [TestMethod]
        public void TestFileMetasFoundPerms()
        {
            using (GetFileMetadatasWrapper(out IntPtr metas, iexploreFolder, out int metasCount))
            {
                int structSize = Marshal.SizeOf(typeof(FileMetadata));
                IntPtr metasTemp = metas;
                string filePerms = "not found";
                for (int i = 0; i < metasCount; ++i)
                {
                    FileMetadata fm = (FileMetadata)Marshal.PtrToStructure(metasTemp, typeof(FileMetadata));
                    metasTemp += structSize;
                    if (fm.name == "images") filePerms = fm.permissions;
                }
                Assert.AreEqual("r-xr-xr-x", filePerms);
            }
        }

        [TestMethod]
        public void TestFileMetasFoundDepth()
        {
            using (GetFileMetadatasWrapper(out IntPtr metas, iexploreFolder, out int metasCount))
            {
                int structSize = Marshal.SizeOf(typeof(FileMetadata));
                IntPtr metasTemp = metas;
                uint depth = 0;
                for (int i = 0; i < metasCount; ++i)
                {
                    FileMetadata fm = (FileMetadata)Marshal.PtrToStructure(metasTemp, typeof(FileMetadata));
                    metasTemp += structSize;
                    if (fm.name == "bing.ico") depth = fm.depth;
                }
                Assert.AreEqual(1u, depth);
            }
        }

        [TestMethod]
        public void TestFileMetasNonExistent()
        {
            try
            {
                using (GetFileMetadatasWrapper(out IntPtr metas, "nowhere", out int metasCount))
                {
                }
                Assert.Fail("An exception should be thrown.");
            }
            catch (SEHException)
            {
            }
        }
    }
}
