using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace MyApplicationDerp
{
    class Character : INotifyPropertyChanged
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

        public Int32 Gold
        {
            get { return _Gold; }
            set
            {
                this.UpdateValue(value, ref _Gold, "Gold");
            }
        }
        private Int32 _Gold;

        private void UpdateValue<T>(T newValue, ref T memberValue, String name)
        {
            if (newValue.Equals(memberValue))
                return;

            this.SendPropertyChanged(name);
            memberValue = newValue;
        }

        private void SendPropertyChanged(String name)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(name));
            }
        }

        //public Int32 Lives;
        //public String Residence;
        //public Int32 Age;
        //public Int32 Experience;
        //public Race Race;

        public event PropertyChangedEventHandler PropertyChanged;
    }

    enum Race
    {
        Orc,
        Human,
        Elf
    }
}
