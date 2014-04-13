using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace MyApplicationDerp
{
    class PropertyNotifier : INotifyPropertyChanged
    {
        public void UpdateValue<T>(T newValue, ref T memberValue, String name)
        {
            if (newValue.Equals(memberValue))
                return;

            memberValue = newValue;
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(name));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }

    class Weapon : PropertyNotifier
    {
        public String Name
        {
            get { return _Name; }
            set
            {
                this.UpdateValue(value, ref _Name, "Name");
            }
        }
        private String _Name;

        public Int32 LivesTaken
        {
            get { return _LivesTaken; }
            set
            {
                this.UpdateValue(value, ref _LivesTaken, "LivesTaken");
            }
        }
        private Int32 _LivesTaken;
    }

    class Character : PropertyNotifier
    {
        public Weapon CurrentWeapon { get; set; }

        public List<Weapon> Weapons { get; set; }
        
        public String Name
        {
            get { return _Name; }
            set
            {
                this.UpdateValue(value, ref _Name, "Name");
            }
        }
        private String _Name;

        public Int32 Gold
        {
            get { return _Gold; }
            set
            {
                this.UpdateValue(value, ref _Gold, "Gold");
            }
        }
        private Int32 _Gold;

        public void KillMe()
        {
        }

        //public Int32 Lives;
        //public String Residence;
        //public Int32 Age;
        //public Int32 Experience;
        //public Race Race;
    }

    enum Race
    {
        Orc,
        Human,
        Elf
    }
}
