from flaskr import create_app


from flaskwebgui import FlaskUI # import FlaskUI
app = create_app()
ui = FlaskUI(app, width=500, height=500)
if __name__ == "__main__":
    print("hello all")
    print("hssello all")
    ui.run()
    # app.run(debug=True)
# to run the app