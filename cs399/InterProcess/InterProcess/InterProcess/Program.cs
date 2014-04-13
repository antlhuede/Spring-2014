using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace InterProcess
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.WriteLine("Starting");
			var socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			
			Console.WriteLine("Connecting to host");

			socket.Connect("localhost", 8000);

			Console.WriteLine("Sending data");

			var stream = new MemoryStream();
			var writer = new BinaryWriter(stream);

			writer.Write(Encoding.UTF8.GetBytes("Hello"));

			socket.Send(stream.ToArray());

			for (; ; )
			{
				if (!socket.Connected)
				{
					Console.WriteLine("Disconnected");
					break;
				}

				Byte[] data = new Byte[4096];
				if (socket.Available != 0)
				{
					int count = socket.Receive(data);

					String text = Encoding.UTF8.GetString(data, 0, count);
					Console.WriteLine("Received: " + text);
				}
				
				// Editor loop! Though normally we don't control the editor loop,
				// so we need to refactor this to be called on a periodic update by the editor
				Thread.Sleep(16);
			}

			Console.WriteLine("Exiting");

			Console.WriteLine("Press any key to continue...");
			Console.ReadLine();
		}
	}
}
