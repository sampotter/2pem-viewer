sig = tcp_source {
   hostname = "localhost",
   port = 8888
}

sig = rank_filter {
   input = sig,
   order = 2,
   length = 5
}

pyr = image_pyramid {
   input = sig,
   depth = -1
}

dog = difference_of_guassians { input = CLAHE(pyr) }

--
-- we want to stream the detected blobs to an OSC channel
--
osc {
   input = dog, 
   channel = "/data/blobdetect"
}

--
-- we want to display the rank filtered input
--
window {
   input = sig, 
   title = "test"
}
