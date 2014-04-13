using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.ComponentModel;

namespace DatabindingExamples
{
  public partial class MainPage : UserControl
  {
    private EventModel _event;

    public MainPage()
    {
      InitializeComponent();

      // create a model object
      _event = new EventModel()
      {
        Date = new DateTime(2011, 7, 1),
        Title = "Silverlight User Group"
      };

      // copy our model object state to the UI
      EventDate.SelectedDate = _event.Date;
      EventTitle.Text = _event.Title;

      // 'listen' to changes in the model
      _event.PropertyChanged += Event_PropertyChanged;
    }

    /// <summary>
    /// Handles changes in the model, reflecting this change in the UI (view)
    /// </summary>
    private void Event_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      if (e.PropertyName == "Title")
      {
        EventTitle.Text = _event.Title;
      }
      if (e.PropertyName == "Date")
      {
        EventDate.SelectedDate = _event.Date;
      }
    }

    /// <summary>
    /// Handles the TextChanged event updating the model accordingly
    /// </summary>
    private void EventTitle_TextChanged(object sender, TextChangedEventArgs e)
    {
      _event.Title = EventTitle.Text;
    }

    /// <summary>
    /// Handles the SelectedDateChanged event updating the model accordingly
    /// </summary>
    private void EventDate_SelectedDateChanged(object sender, SelectionChangedEventArgs e)
    {
      _event.Date = EventDate.SelectedDate.Value;
    }

    private void Button_Click(object sender, RoutedEventArgs e)
    {
      _event.Title = _event.Title.ToLower();
      _event.Date = _event.Date.AddDays(1);
    }
  }
}
