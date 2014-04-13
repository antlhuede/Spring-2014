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

namespace MyApplicationDerp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Character MyCharacter;

        public MainWindow()
        {
            InitializeComponent();

            MyCharacter = new Character()
            {
                Name = "Richard III",
                Gold = 10,
                CurrentWeapon = new Weapon()
                {
                    Name = "Sword of Ten Truths",
                    LivesTaken = 1,
                },

                Weapons = new List<Weapon>()
                {
                    new Weapon()
                    {
                        Name = "Sword of Omens",
                        LivesTaken = 1,
                    },

                    new Weapon()
                    {
                        Name = "Sword of What",
                        LivesTaken = 1,
                    },
                }
            };


            this.DataContext = MyCharacter;
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("Name: " + MyCharacter.Name);
            builder.AppendLine("Gold: " + MyCharacter.Gold);
            builder.AppendLine("CurrentWeapon.Name: " + MyCharacter.CurrentWeapon.Name);
            builder.AppendLine("CurrentWeapon.LivesTaken: " + MyCharacter.CurrentWeapon.LivesTaken);
            this.DebugText.Text = builder.ToString();
        }

        private void GiveGoldButton_Click(object sender, RoutedEventArgs e)
        {
            MyCharacter.Gold += 100;
        }
    }
}
