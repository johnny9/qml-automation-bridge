import cutechimp
import time
chimpy = cutechimp.CuteChimp("localhost", "8080")
chimpy.click({'text':'View 1'})
time.sleep(1)
chimpy.click({"text":"View 2"})
time.sleep(1)
result = chimpy.isVisible({"text":"View 1"})
print(result)
