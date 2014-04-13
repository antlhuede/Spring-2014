using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace DatabindingExample
{
  public class MatchResult : FrameworkElement
  {
    /// <summary>
    /// Gets or sets the total score. This is a dependency property.
    /// </summary>
    public int TotalScore
    {
      set
      {
        SetValue(MatchResult.TotalGoalsProperty, value);
      }
      get
      {
        return (int)GetValue(MatchResult.TotalGoalsProperty);
      }
    }

    /// <summary>
    /// Identifies the TotalGoals Dependency Property.
    /// <summary>
    public static readonly DependencyProperty TotalGoalsProperty =
        DependencyProperty.Register("TotalGoals", typeof(int),
        typeof(MatchResult), new PropertyMetadata(0));

  }
}
