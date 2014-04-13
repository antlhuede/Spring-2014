using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using System.Windows.Threading;
using AvalonDock;

namespace WpfTest
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		DispatcherTimer mUpdateGameTimer;

		GameHost Game;

		public MainWindow()
		{
			ThemeFactory.ChangeTheme(new Uri("/AvalonDock.Themes;component/themes/dev2010.xaml", UriKind.RelativeOrAbsolute));

			InitializeComponent();

			mUpdateGameTimer = new DispatcherTimer();
			mUpdateGameTimer.Interval = TimeSpan.FromSeconds(1.0 / 120.0);
			mUpdateGameTimer.Tick += mUpdateGameTimer_Tick;
			mUpdateGameTimer.IsEnabled = true;

			var dock = new DockableContent();
			Game = new GameHost();
			dock.Content = Game;

			dock.Show(this.dockingManager1);
		}

		void mUpdateGameTimer_Tick(object sender, EventArgs e)
		{
			Game.Update();
		}

		private void button1_Click(object sender, RoutedEventArgs e)
		{
			//MessageBox.Show("Hello!");

			DoStuff();
		}

		[DllImport("SimpleDllTest.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void DoStuff();
	}
}
