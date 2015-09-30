import json
import http.client
import urllib.parse

class CuteChimp:

    def __init__(self, ip, port):
        self.ip = ip
        self.port = port

    def invoke(self, functionName, query):
        client = http.client.HTTPConnection(self.ip+":"+self.port)
        params = urllib.parse.urlencode(query)
        client.request("POST", "/"+functionName+"?"+params)
        response = client.getresponse()
        return response

    def isVisible(self, query):
        response = self.invoke("findItem", query)
        print(response.status)
        if response.status == 200:
            return True
        else:
            return False

    def click(self, query):
        params = urllib.parse.urlencode(query)
        response = self.invoke("click", query)
        if response.status == 200:
            return True
        else:
            return False
