using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace CSharpPluginLoader
{
	class Program
	{
		String mPluginSourceDir;
		String mPluginDestDir;

		void InstanceMain()
		{
			String exePath = Assembly.GetEntryAssembly().Location;
			String exeDir = Path.GetDirectoryName(exePath);
			mPluginSourceDir = Path.Combine(exeDir, @"..\..\..\Plugins");

			mPluginDestDir = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName());
			Directory.CreateDirectory(mPluginDestDir);

			this.CopyPluginsSourceToDest();

			List<ITool> pluginInstances = new List<ITool>();

			foreach (var dllFileName in Directory.EnumerateFiles(mPluginDestDir, "*.dll"))
			{
				if (IsManagedAssembly(dllFileName))
				{
					Assembly loadedAssembly = Assembly.LoadFile(dllFileName);

					bool foundPlugins = false;

					foreach (var type in loadedAssembly.GetTypes())
					{
						// If this type is a tool
						//type.IsAssignableFrom(typeof(ITool))
						if (type.GetInterfaces().Contains(typeof(ITool)))
						{
							var tool = (ITool)Activator.CreateInstance(type);
							pluginInstances.Add(tool);
							foundPlugins = true;
						}
					}

					if (foundPlugins == false)
					{
						Console.WriteLine("Managed dll '" + Path.GetFileName(dllFileName) + "' did not define any plugins!");
					}
				}
				else
				{
					var loadedDll = new UnmanagedLibrary(dllFileName);

					// Is this our type of dll? If not... who the heck is putting random dlls in the plugins folder???
					if (loadedDll.HasUnmanagedFunction("IToolCreate"))
					{
						var tool = new NativeTool(loadedDll);
						pluginInstances.Add(tool);
					}
					else
					{
						Console.WriteLine("Native dll '" + Path.GetFileName(dllFileName) + "' did not define any plugins!");
						loadedDll.Dispose();
					}
				}
			}

			foreach (var plugin in pluginInstances)
			{
				Console.WriteLine("Plugin loaded: " + plugin.Name);
				Console.WriteLine("Value: " + plugin.ComputeValue(55, 109));
			}

			Console.WriteLine("Modify any plugins and press enter...");
			Console.ReadLine();

			//unmanagedLib.Dispose();
			//this.CopyPluginsSourceToDest();
			//unmanagedLib.Reload();

			foreach (var plugin in pluginInstances)
			{
				Console.WriteLine("Plugin loaded: " + plugin.Name);
				Console.WriteLine("Value: " + plugin.ComputeValue(55, 109));
			}

			Console.WriteLine("Press any key to continue...");
			Console.ReadLine();
		}

		static Boolean IsManagedAssembly(String dllFileName)
		{
			try
			{
				AssemblyName.GetAssemblyName(dllFileName);
				return true;
			}
			catch
			{
			}

			return false;
		}

		void CopyPluginsSourceToDest()
		{
			foreach (var dll in Directory.EnumerateFiles(mPluginSourceDir, "*.dll"))
			{
				String destPath = Path.Combine(mPluginDestDir, Path.GetFileName(dll));
				try
				{
					File.Copy(dll, destPath, true);
				}
				catch
				{
				}
			}
		}

		static void Main(string[] args)
		{
			var program = new Program();
			program.InstanceMain();
		}
	}
}
