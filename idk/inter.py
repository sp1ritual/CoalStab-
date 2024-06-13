import dash
from dash import dcc, html, Input, Output
import plotly.graph_objs as go
import dash_bootstrap_components as dbc

# Initialize the Dash app
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP])

# Define sidebar layout
sidebar = html.Div(
    [
        html.Img(src='logo.png.png', style={'width': '100px', 'margin': '10px'}),
        html.Hr(),
        dbc.Nav(
            [
                dbc.NavLink("Список датчиков в системе", href="/", active="exact", id="nav-sensors"),
                dbc.NavLink("Аналитика собранных данных", href="/analytics", active="exact", id="nav-analytics"),
                dbc.NavLink("Уведомления", href="/notifications", active="exact", id="nav-notifications"),
                dbc.NavLink("Обратная связь", href="/feedback", active="exact", id="nav-feedback"),
                dbc.NavLink("Настройки", href="/settings", active="exact", id="nav-settings"),
            ],
            vertical=True,
            pills=True,
        ),
        html.Div(
            [
                html.Div("CoalStab", style={'margin-top': '20px'}),
                html.Div("coalstab@mail.ru")
            ],
            style={'position': 'absolute', 'bottom': '10px', 'left': '10px'}
        ),
    ],
    style={
        'padding': '10px',
        'width': '200px',
        'position': 'fixed',
        'height': '100%',
        'background-color': '#f8f9fa'
    }
)

# Define content layout
content = html.Div(id='page-content', style={'margin-left': '220px', 'padding': '10px'})

# Define layout
app.layout = html.Div([dcc.Location(id="url"), sidebar, content])

# Define the sensors page layout
sensors_layout = html.Div(
    [
        html.H1("Список датчиков в системе", style={'text-align': 'center'}),
        # Example content for the sensors page
        html.P("Здесь будет список датчиков.")
    ]
)

# Define the analytics page layout
analytics_layout = html.Div(
    [
        html.H1("Аналитика собранных данных", style={'text-align': 'center'}),

        # Line chart
        dcc.Graph(
            id='line-chart',
            figure={
                'data': [
                    go.Scatter(
                        x=['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
                        y=[1, 3, 2, 5, 4, 6, 7, 8, 6, 9, 5, 10],
                        mode='lines',
                        name='Температура'
                    ),
                    go.Scatter(
                        x=['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
                        y=[2, 4, 3, 4, 6, 5, 6, 9, 7, 8, 9, 11],
                        mode='lines',
                        name='Влажность'
                    ),
                    go.Scatter(
                        x=['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
                        y=[3, 2, 5, 6, 7, 8, 6, 7, 9, 10, 11, 12],
                        mode='lines',
                        name='Глубина откоса отвала'
                    )
                ],
                'layout': go.Layout(
                    title='',
                    xaxis={'title': ''},
                    yaxis={'title': ''},
                    margin={'l': 20, 'r': 10, 't': 10, 'b': 20}
                )
            }
        ),

        # Cards with current values
        dbc.Row(
            [
                dbc.Col(
                    dbc.Card(
                        dbc.CardBody(
                            [
                                html.H5("Температура", className="card-title"),
                                html.P("68°С", className="card-text"),
                            ]
                        ),
                    ),
                    width=4
                ),
                dbc.Col(
                    dbc.Card(
                        dbc.CardBody(
                            [
                                html.H5("Влажность", className="card-title"),
                                html.P("70%", className="card-text"),
                            ]
                        ),
                    ),
                    width=4
                ),
                dbc.Col(
                    dbc.Card(
                        dbc.CardBody(
                            [
                                html.H5("Глубина откоса отвала", className="card-title"),
                                html.P("35°", className="card-text"),
                            ]
                        ),
                    ),
                    width=4
                ),
            ]
        ),

        # Bar chart
        dcc.Graph(
            id='bar-chart',
            figure={
                'data': [
                    go.Bar(x=['A', 'B', 'C'], y=[4, 5, 6], name='Скольжение'),
                    go.Bar(x=['A', 'B', 'C'], y=[3, 4, 5], name='Трение')
                ],
                'layout': go.Layout(
                    title='',
                    barmode='group',
                    xaxis={'title': ''},
                    yaxis={'title': ''},
                    margin={'l': 20, 'r': 10, 't': 10, 'b': 20}
                )
            }
        ),

        # Pie chart
        dcc.Graph(
            id='pie-chart',
            figure={
                'data': [
                    go.Pie(labels=['Данные 1', 'Данные 2', 'Данные 3'], values=[40, 30, 30])
                ],
                'layout': go.Layout(
                    title='',
                    margin={'l': 20, 'r': 10, 't': 10, 'b': 20}
                )
            }
        ),
    ]
)

# Define the notifications page layout
notifications_layout = html.Div(
    [
        html.H1("Уведомления", style={'text-align': 'center'}),
        # Example content for the notifications page
        html.P("Здесь будут уведомления.")
    ]
)

# Define the feedback page layout
feedback_layout = html.Div(
    [
        html.H1("Обратная связь", style={'text-align': 'center'}),
        # Example content for the feedback page
        html.P("Здесь будет форма обратной связи.")
    ]
)

# Define the settings page layout
settings_layout = html.Div(
    [
        html.H1("Настройки", style={'text-align': 'center'}),
        # Example content for the settings page
        html.P("Здесь будут настройки.")
    ]
)


# Update page content based on URL
@app.callback(Output('page-content', 'children'), [Input('url', 'pathname')])
def display_page(pathname):
    if pathname == '/analytics':
        return analytics_layout
    elif pathname == '/notifications':
        return notifications_layout
    elif pathname == '/feedback':
        return feedback_layout
    elif pathname == '/settings':
        return settings_layout
    else:
        return sensors_layout


# Run the app
if __name__ == '__main__':
    app.run_server(debug=True)

