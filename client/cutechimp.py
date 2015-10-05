import json
import httplib
import urllib

class CuteChimp:

    def __init__(self, ip, port):
        self.ip = ip
        self.port = port

    def invoke(self, functionName, query):
        client = httplib.HTTPConnection(self.ip+":"+self.port)
        params = urllib.urlencode(query)
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
        params = urllib.urlencode(query)
        response = self.invoke("click", query)
        if response.status == 200:
            return True
        else:
            return False
