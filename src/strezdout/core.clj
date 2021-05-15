(ns strezdout.core
  (:require [ring.adapter.jetty :refer [run-jetty]]
            [ring.util.request :refer [body-string]]
            [ring.util.response :refer [response]]
            [strezdout.strez :refer [read-category]])
  (:import  [java.io BufferedReader StringReader]))

(defn handler [request]
  (response (str (read-category (line-seq (BufferedReader. (StringReader. (body-string request))))))))

(defn -main []
  (run-jetty handler {:port 8080}))
