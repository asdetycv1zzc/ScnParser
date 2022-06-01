using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.IO;

namespace ScnViewer
{
    public class Parser
    {
        public class ScnSingleString
        {
            public System.UInt64 _beginPos;
            public System.UInt64 _endPos;
            public System.UInt64 _real_size;
            public System.UInt64 _memory_size;
            public System.UInt64 _in_file_size;
            public String _Content;
        }
        public class ScnString
        {
            public String Speaker;
            public List<ScnSingleString> Content;
        };
        private IntPtr _pointer = IntPtr.Zero;
        [DllImport("N:\\Users\\John\\source\\repos\\PsbParser\\Debug\\ScnParser.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern IntPtr EstablishPointer();
        [DllImport("N:\\Users\\John\\source\\repos\\PsbParser\\Debug\\ScnParser.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern bool Init(IntPtr ptr, StringBuilder _FileAddress);
        [DllImport("N:\\Users\\John\\source\\repos\\PsbParser\\Debug\\ScnParser.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern bool Parse(IntPtr ptr, [MarshalAs(UnmanagedType.LPArray)] byte[] _dest, ref ulong _size);

        public bool Init(StringBuilder _FileAddress)
        {
            return Init(_pointer, _FileAddress);
        }
        public List<ScnString> Parse()
        {
            byte[] _temp = null;
            ulong _size = 0;
            Parse(_pointer, _temp, ref (_size));
            byte[] _result_in_byte = new byte[_size];
            Parse(_pointer,_result_in_byte, ref _size);

            MemoryStream resultStream = new MemoryStream(_result_in_byte);
            BinaryReader binaryStream = new BinaryReader(resultStream);

            UInt64 _Count = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)),0);

            List<ScnString> _result = new List<ScnString>();
            for (ulong i = 0; i < _Count; i++)
            {
                _result.Add(new ScnString());
                _result[(int)i].Content = new List<ScnSingleString>();
            }

            for (UInt64 i = 0;i < _Count;i++)
            {
                UInt64 _speaker_size = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)),0);
                _result[(int)i].Speaker = Encoding.Unicode.GetString(binaryStream.ReadBytes((int)_speaker_size));
                
                UInt64 _content_count = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)), 0);

                for(UInt64 j = 0;j < _content_count;j++)
                {
                    ScnSingleString _single_string = new ScnSingleString
                    {
                        _beginPos = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)), 0),
                        _endPos = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)), 0),
                        _real_size = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)), 0),
                        _memory_size = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)), 0),
                        _in_file_size = BitConverter.ToUInt32(binaryStream.ReadBytes(sizeof(UInt32)), 0)
                    };
                    _single_string._Content = Encoding.Unicode.GetString(binaryStream.ReadBytes((int)_single_string._memory_size));
                    _result[(int)i].Content.Add(_single_string);
                }
            }
            binaryStream.Dispose();
            resultStream.Dispose();
            return _result;
        }
        public Parser()
        {
            _pointer = EstablishPointer();
        }
        public Parser(StringBuilder _fileAddress)
        {
            _pointer = EstablishPointer();
            Init(_pointer, _fileAddress);
        }
    }

}
