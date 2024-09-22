from flask import Flask
from flask_login import LoginManager

def create_app():
    app = Flask(__name__)
    
    # Load configuration
    app.config.from_object('config.config.Config')
    
    # Initialize Login Manager
    login_manager = LoginManager()
    login_manager.init_app(app)
    login_manager.login_view = 'login'
    
    # Register Blueprints or Routes
    from .routes import main as main_blueprint
    app.register_blueprint(main_blueprint)
    
    return app