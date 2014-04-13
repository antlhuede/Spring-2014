using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CSharpPluginLoader
{
	// .NET and Com interface (simple and happy :D)
	public interface ITool : IDisposable
	{
		string Name { get; }
		int ComputeValue(int x, int y);
	}

	// Native dll (many languages and compilers can export this format! C, C++, D, Delphi...)
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate IntPtr IToolCreate();

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void IToolDestroy(IntPtr instance);
	
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate string IToolGetName(IntPtr instance);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate int IToolComputeValue(IntPtr instance, int x, int y);

	public class NativeTool : ITool
	{
		IntPtr mInstance;

		IToolGetName mIToolGetName;
		IToolComputeValue mIToolComputeValue;

		UnmanagedLibrary mLibrary;

		public NativeTool(UnmanagedLibrary lib)
		{
			mLibrary = lib;

			var creator = mLibrary.GetUnmanagedFunction<IToolCreate>("IToolCreate");
			mInstance = creator();

			mIToolGetName = mLibrary.GetUnmanagedFunction<IToolGetName>("IToolGetName");
			mIToolComputeValue = mLibrary.GetUnmanagedFunction<IToolComputeValue>("IToolComputeValue");
		}

		public string Name
		{
			get { return mIToolGetName(mInstance); }
		}

		public int ComputeValue(int x, int y)
		{
			return mIToolComputeValue(mInstance, x, y);
		}

		public void Dispose()
		{
			var destructor = mLibrary.GetUnmanagedFunction<IToolDestroy>("IToolDestroy");
			destructor(mInstance);
		}
	}
}
