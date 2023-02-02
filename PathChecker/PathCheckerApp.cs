using Microsoft.Win32.SafeHandles;
using System;
using System.Runtime.InteropServices;

namespace PathChecker
{
    public class PathCheckerApp
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct FileMetadata
        {
            public uint depth;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string name;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 23)]
            public string type;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 10)]
            public string size;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string lastWriteTime;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 10)]
            public string permissions;
        }

        [DllImport(@"D:\PathChecker\x64\Debug\FileMetadata.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetFileMetadatas(out FileMetadatasSafeHandle metasHandle, out IntPtr metas, string path, out int metasCount);

        [DllImport(@"D:\PathChecker\x64\Debug\FileMetadata.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool ReleaseFileMetadatas(IntPtr metasHandle);

        public class FileMetadatasSafeHandle : SafeHandleZeroOrMinusOneIsInvalid
        {
            public FileMetadatasSafeHandle() : base(true) { }

            protected override bool ReleaseHandle()
            {
                return ReleaseFileMetadatas(handle);
            }
        }

        public static FileMetadatasSafeHandle GetFileMetadatasWrapper(out IntPtr metas, string path, out int metasCount)
        {
            return !GetFileMetadatas(out FileMetadatasSafeHandle metasHandle, out metas, path, out metasCount)
                ? throw new InvalidOperationException()
                : metasHandle;
        }

        static void Main(string[] args)
        {
            try
            {
                using (GetFileMetadatasWrapper(out IntPtr metas, Console.ReadLine(), out int metasCount))
                {
                    int structSize = Marshal.SizeOf(typeof(FileMetadata));
                    IntPtr metasTemp = metas;
                    for (int i = 0; i < metasCount; ++i)
                    {
                        FileMetadata fm = (FileMetadata)Marshal.PtrToStructure(metasTemp, typeof(FileMetadata));
                        metasTemp += structSize;
                        Console.WriteLine(string.Concat(System.Linq.Enumerable.Repeat("  ", (int)fm.depth)) + fm.name + " - <" + fm.type + "> - " + fm.size + " - " + fm.lastWriteTime + " - " + fm.permissions);
                    }
                }
            }
            catch (SEHException ex)
            {
                Console.WriteLine(ex.Message);
            }
            Console.ReadLine();
        }
    }
}
