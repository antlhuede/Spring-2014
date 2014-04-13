using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CSharpPluginLoader;

namespace CSharpPlugin
{
	public class GeometryPainter : ITool
	{
		public string Name
		{
			get { return "Geometry Painter"; }
		}

		public int ComputeValue(int x, int y)
		{
			return x + y;
		}

		public void Dispose()
		{
			// Nothing to do :D
		}
	}
}
