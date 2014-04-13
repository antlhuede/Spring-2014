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
using System.Windows.Data;

namespace DatabindingExample
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

      // bind the Date to the UI
      EventDate.SetBinding(DatePicker.SelectedDateProperty, new Binding("Date")
      {
        Source = _event,
        Mode = BindingMode.TwoWay
      });

      // bind the Title to the UI

      FrameworkElement targetObject = EventTitle;
      DependencyProperty targetProperty = TextBox.TextProperty;
      object sourceObject = _event;
      string sourceProperty = "Title";

      var binding = new Binding(sourceProperty)
      {
        Source = sourceObject,
        Mode = BindingMode.TwoWay
      };
      targetObject.SetBinding(targetProperty, binding);
      BindingExpression exp = targetObject.GetBindingExpression(targetProperty);
    }

    private void Button_Click(object sender, RoutedEventArgs e)
    {
      _event.Title = _event.Title.ToLower();
      _event.Date = _event.Date.AddDays(1);
    }
  }
}
