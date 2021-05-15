(ns strezdout.fileutil
  (:require [clojure.java.io :refer [file]]
             [clojure.java.shell :refer [sh]]))

(defn canonize [& path] (.getCanonicalPath (apply file path)))

(def testers-canonize (partial canonize "resources" "testers"))

(defn mkdir! [path] (sh "mkdir" "-p" path))

(defn rm! [path] (sh "rm" "-rf" path))

(defn copy! [from to] (sh "cp" "-r" from to))

(defn cmake! [build source] (sh "cmake" "-B" build "-S" source))

(defn make! [build target] (sh "make" "-C" build target))
