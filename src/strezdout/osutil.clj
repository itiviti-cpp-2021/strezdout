(ns strezdout.osutil
  (:require [clojure.java.io :refer [file]]
            [clojure.java.shell :refer [sh]]
            [clojure.string :as string]))

(defn join [& path] (apply file path))

(defn canonize [& path] (.getCanonicalPath (apply file path)))

(def testers-canonize (partial canonize "resources" "testers"))

(defn mkdir! [path] (sh "mkdir" "-p" path))

(defn tmpdir! [] (string/trim (:out (sh "mktemp" "-d"))))

(defn rm! [path] (sh "rm" "-rf" path))

(defn copy! [from to] (println (sh "cp" "-R" from to)))

(defn cmake! [build source] (println (sh "cmake" "-B" build "-S" source)))

(defn make! [build target] (sh "make" "-C" build target))
