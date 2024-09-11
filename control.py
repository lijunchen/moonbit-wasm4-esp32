import requests

def player_1_up():
    url = 'http://192.168.31.194/hello?player=0&state=64'
    r = requests.get(url)
    print(r.text)

if __name__ == "__main__":
    player_1_up()