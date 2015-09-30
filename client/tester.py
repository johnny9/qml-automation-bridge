import cutechimp
import time
chimpy = cutechimp.CuteChimp("localhost", "8080")
chimpy.invoke("click", {'text':'View 1'})
dictionary = {}
dictionary["text"] = 'View 2'
dictionary
time.sleep(1)
chimpy.click(dictionary)
time.sleep(1)
result = chimpy.isVisible({"text":"View 1"})
print(result)
