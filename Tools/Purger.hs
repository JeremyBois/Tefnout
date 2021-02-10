module Main where

import Control.Monad
import System.Directory
import System.Environment

-- WARNING
-- <getDirectoryContents> should be avoid because it includes special entries (. and ..)
-- <listDirectory> is safer
--

main :: IO ()
main = do
  args <- getArgs
  at_project_root <- doesFileExist "CMakeLists.txt"

  -- This script should only be run from root
  if at_project_root
    then do
      -- Purge clangd cache
      when ("clangd" `elem` args) purgeClangd

      -- Purge cmake generated files
      when ("cmake" `elem` args) purgeCmake

      -- Purge tefnout logs
      when ("tefnout" `elem` args) purgeTefnout
    else do
      putStr "Provided arguments are: " >> print args
      currentDir <- getCurrentDirectory
      print currentDir
      putStrLn "ERROR - This script must be run only from Tefnout project directory."

purgeCmake :: IO ()
purgeCmake = do
  forM_ paths removeIfExists
  where
    paths = ["bin", "lib", "build"]

purgeClangd :: IO ()
purgeClangd = forM_ paths removeIfExists
  where
    paths = [".cache", "compile_commands.json", "build/compile_commands.json"]

purgeTefnout :: IO ()
purgeTefnout = forM_ paths removeIfExists
  where
    paths = ["logs"]

rmFile :: FilePath -> IO ()
rmFile file = print ("Deleting File - " <> file) >> removeFile file

rmFolder :: FilePath -> IO ()
rmFolder file = print ("Deleting Folder - " <> file) >> removeDirectoryRecursive file

removeIfExists :: FilePath -> IO ()
removeIfExists path = do
  pathExist <- doesPathExist path
  when pathExist $ do
    isDirectory <- doesDirectoryExist path
    if isDirectory
      then rmFolder path
      else rmFile path

-- Currently not used
-- rmWithinFolder :: FilePath -> IO ()
-- rmWithinFolder path = do
--   isDirectory <- doesDirectoryExist path
--   when isDirectory $ do
--     paths <- listDirectory path
--     forM_ paths removeIfExists
