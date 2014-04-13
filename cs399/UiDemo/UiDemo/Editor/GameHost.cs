using System.Runtime.InteropServices;
using System.Windows.Interop;
using System;

namespace WpfTest
{
	public class GameHost : HwndHost
	{
		[DllImport("DllPlugin.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr CreateGame(IntPtr parentWindow);

		[DllImport("DllPlugin.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void DestroyGame();

		[DllImport("DllPlugin.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void UpdateGame();

		[DllImport("DllPlugin.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr WindowProcedure(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam);

		public void Update()
		{
			UpdateGame();
		}

		protected override HandleRef BuildWindowCore(HandleRef hwndParent)
		{
			IntPtr childWindow = CreateGame(hwndParent.Handle);
			return new HandleRef(this, childWindow);
		}

		protected override void DestroyWindowCore(HandleRef hwnd)
		{
			DestroyGame();
		}
	}
}


