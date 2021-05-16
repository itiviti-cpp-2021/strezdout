(ns strezdout.osutil
  (:require [clojure.java.io :as io]
            [clojure.java.shell :refer [sh]]
            [clojure.string :as string]))

(defn sh-log-wrapper [f] #(println (:out (apply f %&))))

(defn join [& path] (apply io/file path))

(defn canonize [& path] (.getCanonicalPath (apply io/file path)))

(def testers-canonize (partial canonize "resources" "testers"))

(defn mkdir! [path] (sh "mkdir" "-p" path))

(defn tmpdir! [] (string/trim (:out (sh "mktemp" "-d"))))

(defn rm! [path] (sh "rm" "-rf" path))

(defn copy! [from to] (sh "cp" "-R" from to))

(defn cmake! [build source] (sh "cmake" "-B" build "-S" source))

(defn make! [build target] (sh "make" "-C" build target))
