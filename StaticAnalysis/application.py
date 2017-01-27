from pymongo import MongoClient
import boto.s3.connection
import re
import os
import subprocess
from flask import Flask, render_template, request, redirect, session, flash, url_for, Markup
from werkzeug.security import generate_password_hash, check_password_hash

username = None

# DataBase Connection
class MongoDB():
    def __init__(self):
        client = MongoClient('mongodb://ds113668.mlab.com:13668')
        self.db = client['awsstaticanalysisdb']
        self.db.authenticate('sagar', 'passw0rd')

    def get_collection(self,collection_name):
        return self.db[collection_name]
print "DB Connected Successfully"

application = Flask(__name__)
app = application

@app.route('/')
def index():
    if 'username' in session:
        return render_template('index.html', user=session['username'])
    else:
        return render_template('login.html')

# Index (Home) Page
@app.route('/home')
def home():
    if 'username' in session:
        return render_template('index.html', title='Home Page',  user=session['username'])
    else:
        return render_template('login.html')

# About Page
@app.route('/about')
def about():
    if 'username' in session:
        return render_template('about.html', title='About', message='Project description page')
    else:
        return render_template('login.html')


@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.method == 'GET':
        return render_template('login.html')
    global username
    User_Name = request.form['signupusername']
    Passowrd = request.form['signuppassword']


    #UserName Validation Code
    isValidUserName = re.match("^[a-zA-Z0-9]+([a-zA-Z0-9](_|-| )[a-zA-Z0-9])*[a-zA-Z0-9]+$", User_Name)
    print isValidUserName
    if not isValidUserName:
        return render_template('login.html', SignupMessage="Invalid Username")

    client = MongoDB()
    UserCollection = client.get_collection('Users')
    query = UserCollection.find({"UserName": User_Name})
    for post in query:
        if post['UserName'] == User_Name:
            return render_template('login.html', SignupMessage="Someone already has that username, please try another")

    #Password Validation Code
    if len(Passowrd) < 8:
        # Password to short
        return render_template('login.html', SignupMessage="Your password must be 8 characters long.")
    elif not re.findall(r'\d+', Passowrd):
        # Missing a number
        return render_template('login.html', SignupMessage="You need a number in your password.")
    elif not re.findall(r'[A-Z]+', Passowrd):
        return render_template('login.html', SignupMessage="You need a capital letter in your password.")
    elif not re.findall(r'[!@#$%^&*()+=]+', Passowrd):
        return render_template('login.html', SignupMessage="You need a special letter in your password.")
    elif not re.match(r'[A-Za-z0-9!@#$%^&*()]{8,}', Passowrd):
        return render_template('login.html', SignupMessage="Invalid Character in password")
    else:
        print("All good")

    hashPassword = generate_password_hash(Passowrd)

    client=MongoDB()
    UserCollection=client.get_collection('Users')
    query = {"UserName": User_Name, "Password": hashPassword}
    UserCollection.insert(query)
    return render_template('login.html', SignupMessage="Awesome! SignUp Successfully.")

# Login Page
@app.route('/login', methods=['GET','POST'])
def do_admin_login():
    if request.method == 'GET':
        return render_template('login.html')
    global username
    User_Name = request.form['username']
    Password = request.form['password']
    username = User_Name

    client=MongoDB()
    UserCollection=client.get_collection('Users')
    query = UserCollection.find({"UserName": username})
    for post in query:
        BoolPassweord = check_password_hash(post['Password'], Password)
        if BoolPassweord and ('username in session'):
            session['username'] = username
            return render_template('index.html', user=username)
    return render_template('login.html', message="Invalid username/password")

# Upload Document
@app.route('/uploadFile', methods=['POST', 'GET'])
def uploadFile():
    if request.method == 'POST' and ('username in session'):
        try:
            global username
            User_Name = request.form['username']
            file_name = request.files['file_upload'].filename
            content = request.files['file_upload'].read()

            #Creating a copy of uploaded file for Static Analysis
            with open(os.path.join('/home/ec2-user/test', file_name), "w") as file1:
                file1.write(content)

            dirPath = "/home/ec2-user/test/"
            filePath = dirPath + file_name

            # FlawFinder Analysis
            commandTmpFlawFinder = "/home/ec2-user/flawfinder-1.31/./flawfinder "
            commandFlawFinder = commandTmpFlawFinder + filePath
            processFlawFinder = subprocess.Popen(commandFlawFinder, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
            # Launch the shell command:
            flawFinderOutput, error = processFlawFinder.communicate()

            # Rats Analysis
            commandTmpRats = "rats --quiet --html -w 3 "
            commandRats = commandTmpRats + filePath
            processRats = subprocess.Popen(commandRats, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
            # Launch the shell command:
            ratsOutput, error = processRats.communicate()


            FileExtension = file_name.endswith((".py", ".cpp", ".c", ".C", ".pl"))

            if file_name.endswith((".c", ".C", ".cpp")):
                postfile = {"UserName": User_Name, "FileName": file_name, "FileContent": content,
                            "RatsOutput": ratsOutput, "FlawFinderOutput": flawFinderOutput}
            else:
                postfile = {"UserName": User_Name, "FileName": file_name, "FileContent": content,
                            "RatsOutput": ratsOutput, "FlawFinderOutput": "Flawfinder is not supported."}

            if FileExtension:
                client = MongoDB()
                DocCollection = client.get_collection('Documents')
                post_id = DocCollection.insert(postfile)
                result = "File Successfully Uploaded"
            else:
                result="Invalid File"
            return render_template("index.html", output=result, user=User_Name)
        except:
            result = "Upload failed"
            return render_template("index.html", output=result)

# View Document Page
@app.route('/getmydocuments', methods=['POST', 'GET'])
def getmydocuments():
    try:
        global username
        if (request.method == 'POST' or request.method == 'GET') and ('username in session'):
            username = session['username']
        else:
            username = username


        # Initializing object of the database class
        client = MongoDB()
        DocCollection = client.get_collection('Documents')
        query = DocCollection.find({"UserName": username})

        returnString = []
        for post in query:
            user_name = post['UserName']
            File_name = post['FileName']
            DocData = post['FileContent']
            RatsOP = post['RatsOutput']
            FlawFinderOP = post['FlawFinderOutput']

            SendData = {"UserName": user_name, "FileName": File_name, "FileContent": DocData, "RatsOutput": Markup(RatsOP), "FlawFinderOutput" : Markup(FlawFinderOP)}
            returnString.append(SendData)
        return render_template("getalldocuments.html", output=returnString)
    except:
        result = "Fail to list files"
        return render_template("index.html", output=result)

# Delete Single Document
@app.route('/deletefile', methods=['POST', 'GET'])
def deletefile():
    User_Name = request.form['username']
    File_Name = request.form['filename']
    try:
        if User_Name == username:
            # Initializing object of the database class
            client = MongoDB()
            DocCollection = client.get_collection('Documents')
            query = DocCollection.remove({"UserName": User_Name, "FileName": File_Name})
            result = 'File Deleted'
            return render_template("index.html", output=result, user=session['username'])
        else:
            result = 'Cannot delete file of other user'
            return render_template("index.html", output=result, user=session['username'])
    except:
        result = "Fail to delete file"
        return render_template("index.html", output=result, user=session['username'])

# Logout Page
@app.route('/logout')
def logout():
    # remove the username from the session if it's there
    global username
    print username
    session.pop('username', None)
    return render_template("login.html", message='Logged out successfully')

# Set the secret key.
app.secret_key = os.urandom(24)

if __name__ == "__main__":
    app.run('ec2-35-163-2-175.us-west-2.compute.amazonaws.com')

